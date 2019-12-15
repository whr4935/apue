#include <gst/gst.h>
#include <stdio.h>
#include <fcntl.h>

#define GST_TYPE_DUMP_HELPER \
  (gst_dump_helper_get_type())
#define GST_DUMP_HELPER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DUMP_HELPER,GstDumpHelper))
#define GST_DUMP_HELPER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_DUMP_HELPER,GstDumpHelperClass))
#define GST_IS_DUMP_HELPER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DUMP_HELPER))
#define GST_IS_DUMP_HELPER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_DUMP_HELPER))


#ifndef MAX_PATH
#define MAX_PATH 260
#endif

typedef struct _GstDumpHelper      GstDumpHelper;
typedef struct _GstDumpHelperClass     GstDumpHelperClass;

struct DumpStream;

struct _GstDumpHelper
{
  GstBin parent;

  GstElement *tee;
  GstElement *src_queue;
  GstElement *dump_queue;
  GstElement *appsink;

  GstPad *sinkpad, *srcpad;
  GstPad *tee_sink_pad;
  GstPad *tee_src_pad;
  GstPad *tee_dump_pad;

  const char dump_dir[MAX_PATH];
  const char source_name[MAX_PATH];
  struct DumpStream *dump_stream;
};

struct _GstDumpHelperClass
{
  GstBinClass parent_class;
};

GType gst_dump_helper_get_type (void);


GST_DEBUG_CATEGORY_STATIC (gst_dump_helper_debug);
#define GST_CAT_DEFAULT gst_dump_helper_debug


/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT
};


static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("ANY")
    );

#define gst_dump_helper_parent_class parent_class
G_DEFINE_TYPE (GstDumpHelper, gst_dump_helper, GST_TYPE_BIN)

/////////////////////////////////////////////////////
static void gst_dump_helper_dispose(GObject *object);
static void gst_dump_helper_finalize (GObject * object);
static void gst_dump_helper_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gst_dump_helper_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static GstStateChangeReturn gst_dump_heleper_change_state (GstElement * element, GstStateChange transition);
static GstStateChangeReturn gst_dump_heleper_change_state(GstElement *element, GstStateChange transition);
static void gst_dump_helper_sink_event(GstPad *pad, GstObject *parent, GstEvent *event);

/////////////////////////////////////////////////////
#define write_dump_stream_pcm write_dump_stream_base
#define write_dump_stream_yuv write_dump_stream_base
#define write_dump_stream_mp3 write_dump_stream_base
#define write_dump_stream_aac write_dump_stream_base
#define write_dump_stream_h264 write_dump_stream_base

static int write_dump_stream_base(struct DumpStream* stream, gchar* pdata, size_t size);

//#undef write_dump_stream_aac
//static int write_dump_stream_aac(struct DumpStream* stream, gchar* pdata, size_t size);

/////////////////////////////////////////////////////
#include "dumpstreamtype.h"

#define KEYWORD(type, write_func) [ DUMP_STREAM_TYPE_##type ] = {#type, write_dump_stream_##type},

static struct {
    const char* suffix;
    int (*write_dump_stream)(struct DumpStream*, const char*, size_t);
} suffix_table[DUMP_STREAM_TYPE_COUNT] = {
    [DUMP_STREAM_TYPE_UNKNOWN] = {"unknown", write_dump_stream_base},
#include "dumpstreamtype.h"
};
#undef KEYWORD


struct DumpStream {
    GstDumpHelper *dumphelper;
    enum DumpStreamType stream_type;
    char suffix[10];
    gboolean is_audio;
    gboolean is_video;

    union {
        struct {
            int width;
            int height;
            const char* pixel_format;
        };

        struct {
            int sample_rate;
            const char* sample_format;
            int channel_number;
        };
    };

    gchar filepath[MAX_PATH];
    int fd;
};

///////////////////
static struct DumpStream* create_dump_stream()
{
    struct DumpStream* stream = NULL;

    stream = (struct DumpStream*)calloc(1, sizeof(struct DumpStream));

    return stream;
}

static void release_dump_stream(struct DumpStream *stream)
{
    free(stream);
}

static enum DumpStreamType get_dump_stream_type(GstStructure *stru)
{
    enum DumpStreamType stream_type = DUMP_STREAM_TYPE_UNKNOWN;

    const gchar* mime = gst_structure_get_name(stru);
    gboolean is_audio = g_str_has_prefix(mime, "audio/");
    gboolean is_video = g_str_has_prefix(mime, "video/");
    const gchar *p = mime + 6;

    if (is_video) {
        if (!strcasecmp(p, "x-raw")) {
            stream_type = DUMP_STREAM_TYPE_yuv;
        } else if (!strcasecmp(p, "x-h264")) {
            stream_type = DUMP_STREAM_TYPE_h264;
        } else {
            GST_DEBUG("mime: %s", mime);
        }
    } else if (is_audio) {
        if (!strcasecmp(p, "x-raw")) {
            stream_type = DUMP_STREAM_TYPE_pcm;
        } else if (!strcasecmp(p, "mpeg")) {
            int version;
            gst_structure_get_int(stru, "mpegversion", &version);
            if (version ==1) {
                stream_type = DUMP_STREAM_TYPE_mp3;
            } else if (version > 1) {
                stream_type = DUMP_STREAM_TYPE_aac;
            }
        }
    }

    if (stream_type >= DUMP_STREAM_TYPE_COUNT)
        stream_type = DUMP_STREAM_TYPE_UNKNOWN;

    return stream_type;
}


static const char* get_dump_stream_suffix(enum DumpStreamType stream_type, const char *mime)
{
    if (stream_type > DUMP_STREAM_TYPE_UNKNOWN && stream_type < DUMP_STREAM_TYPE_COUNT) {
        return suffix_table[stream_type].suffix;
    }

    const char *p = strchr(mime, '/');
    if (p) {
        if (*p == 'x' && *(p+1) == '-') {
            return p+2;
        } else {
            return p;
        }
    }

    return suffix_table[DUMP_STREAM_TYPE_UNKNOWN].suffix;
}

static int init_dump_stream(struct DumpStream *stream, GstStructure* stru, GstDumpHelper *dumphelper)
{
    stream->dumphelper = dumphelper;

    const gchar* mime = gst_structure_get_name(stru);

    stream->is_audio = g_str_has_prefix(mime, "audio/");
    stream->is_video = g_str_has_prefix(mime, "video/");
    stream->stream_type = get_dump_stream_type(stru);
    GST_DEBUG("dump stream type: %d\n", stream->stream_type);
    strcpy(stream->suffix, get_dump_stream_suffix(stream->stream_type, mime));

    switch (stream->stream_type) {
    case DUMP_STREAM_TYPE_pcm:
    case DUMP_STREAM_TYPE_yuv: {
        if (stream->is_audio) {
            const gchar* sample_format = gst_structure_get_string(stru, "format");
            stream->sample_format = g_strdup(sample_format);

            gst_structure_get_int(stru, "rate", &stream->sample_rate);
            gst_structure_get_int(stru, "channels", &stream->channel_number);

            snprintf(stream->filepath, MAX_PATH, "%s/%s_%s_%d_%d.%s", dumphelper->dump_dir, dumphelper->source_name, stream->sample_format, stream->sample_rate, stream->channel_number, stream->suffix);

        } else if (stream->is_video) {
            const gchar* pixel_format = gst_structure_get_string(stru, "format");
            stream->pixel_format = g_strdup(pixel_format);
            gst_structure_get_int(stru, "width", &stream->width);
            gst_structure_get_int(stru, "height", &stream->height);

            snprintf(stream->filepath, MAX_PATH, "%s/%s_%dx%d_%s.%s", dumphelper->dump_dir, dumphelper->source_name, stream->width, stream->height, stream->suffix);
        }
        break;
    }

    default:
        snprintf(stream->filepath, MAX_PATH, "%s/%s.%s", stream->dumphelper->dump_dir, stream->dumphelper->source_name, stream->suffix);
        break;
    }

    return 0;
}

static int open_dump_stream(struct DumpStream *stream)
{
    stream->fd = open(stream->filepath, O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (stream->fd < 0) {
        GST_ERROR("open dumpstream file failed: %s", strerror(errno));
        return -1;
    }
    GST_DEBUG("open dump stream, fd = %d", stream->fd);

    return 0;
}

static int write_dump_stream(struct DumpStream *stream,  const char *pdata, size_t size)
{
    enum DumpStreamType type = stream->stream_type;

    suffix_table[type].write_dump_stream(stream, pdata, size);
}

static int write_dump_stream_base(struct DumpStream* stream, gchar* pdata, size_t size)
{
    int nLeft = size;
    int nWrite = 0;
    int err_count = 0;

    while (nLeft > 0) {
        nWrite = write(stream->fd, pdata, nLeft);
        if (nWrite <= 0) {
            if (errno != EINTR) {
                GST_ERROR("write fd %d error:%s", stream->fd, strerror(errno));
                break;
            }
            nWrite = 0;
            ++err_count;
        }

        if (err_count >= 10) {
            GST_ERROR("write fd %d error!", 1);
            break;
        }

        pdata += nWrite;
        nLeft -= nWrite;
    }

    return size - nLeft;
}

static void close_dump_stream(struct DumpStream* stream)
{
    if (stream->fd > 0) {
        close(stream->fd);
        stream->fd = -1;
    }
}

/////////////////////////////////////////////////////
static void new_sample (GstElement *sink, GstDumpHelper *dumphelper)
{
    GstSample *sample;
    GstBuffer *buffer;
    GstMapInfo map;
    size_t size;

    g_signal_emit_by_name(sink, "pull-sample", &sample);
    if (sample) {
        buffer = gst_sample_get_buffer(sample);
        gst_buffer_map (buffer, &map, GST_MAP_READ);
        if (dumphelper->dump_stream) {
            size = write_dump_stream(dumphelper->dump_stream, map.data, map.size);
            if (size != map.size) {
                GST_WARNING("write dump stream size wrong, size:%d, actual:%d", map.size, size);
            }
        }
        gst_buffer_unmap(buffer, &map);
        gst_buffer_unref(sample);
    }
}

static void received_eos(GstElement *sink, GstDumpHelper *dumphelper)
{
    GST_DEBUG_OBJECT(dumphelper, "appsink recevied EOS!");
  //不能在这里关闭文件
}

static GstPadProbeReturn appsink_sink_pad_probe_cb(GstPad* pad, GstPadProbeInfo *info, GstDumpHelper *dumphelper)
{
    GstPadProbeReturn ret = GST_PAD_PROBE_OK;

    GstEvent *event= gst_pad_probe_info_get_event(info);

    GST_INFO("%s", GST_EVENT_TYPE_NAME(event));
    switch (GST_EVENT_TYPE(event)) {
    case GST_EVENT_CAPS: {
        GstCaps *caps;
        gchar *caps_string;

        gst_event_parse_caps(event, &caps);
        caps_string = gst_caps_to_string(caps);
        GstStructure *stru = gst_caps_get_structure(caps, 0);
        int width, height;
        if (gst_structure_get_int(stru, "width", &width)) {
            GST_INFO("width: %d", width);
        }
        GST_INFO("caps:%s", caps_string);
        g_free(caps_string);

        dumphelper->dump_stream = create_dump_stream();
        if (dumphelper->dump_stream) {
            init_dump_stream(dumphelper->dump_stream, stru, dumphelper);
            open_dump_stream(dumphelper->dump_stream);
        }
        break;
    }
    }

    return ret;
}

/////////////////////////////////////////////////////
/* initialize the plugin's class */
static void
gst_dump_helper_class_init (GstDumpHelperClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass* )(klass);

  gobject_class->set_property = GST_DEBUG_FUNCPTR(gst_dump_helper_set_property);
  gobject_class->get_property = GST_DEBUG_FUNCPTR(gst_dump_helper_get_property);
  gobject_class->dispose = GST_DEBUG_FUNCPTR(gst_dump_helper_dispose);
  gobject_class->finalize = GST_DEBUG_FUNCPTR(gst_dump_helper_finalize);

  gstelement_class->change_state = GST_DEBUG_FUNCPTR(gst_dump_heleper_change_state);


  gst_element_class_set_static_metadata(gstelement_class,
    "pipeline dump helper",
    "Generic/Bin",
    "dump pipeline data to file",
    "multimedia <mm@china-liantong.com>");

  gboolean b =  GST_IS_ELEMENT_CLASS(gstelement_class);
//    GST_DEBUG("b = %d", b);


  gst_element_class_add_pad_template(gstelement_class,
    gst_static_pad_template_get(&sink_factory));
  gst_element_class_add_pad_template(gstelement_class,
    gst_static_pad_template_get(&src_factory));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_dump_helper_init (GstDumpHelper * self)
{
    GstPad *tee_sink_pad;
    GstPad *tee_src_pad;
    GstPad *tee_dump_pad;
    GstPad *src_queue_sink_pad;
    GstPad *src_queue_src_pad;
    GstPad *dump_queue_sink_pad;
    GstPad *dump_queue_src_pad;
    GstPad *appsink_sink_pad;

    tee_sink_pad            =
    tee_src_pad             =
    tee_dump_pad            =
    src_queue_sink_pad      =
    src_queue_src_pad       =
    dump_queue_sink_pad     =
    dump_queue_src_pad      =
    appsink_sink_pad        = NULL;

    self->tee = gst_element_factory_make("tee", "tee");
    self->src_queue = gst_element_factory_make("queue", "src_queue");
    self->dump_queue = gst_element_factory_make("queue", "dump_queue");
    self->appsink = gst_element_factory_make("appsink", "appsink");

    gst_bin_add_many(self, self->tee, self->src_queue, self->dump_queue, self->appsink, NULL);

    tee_sink_pad = gst_element_get_static_pad(self->tee, "sink");
    self->sinkpad = gst_ghost_pad_new("sink", tee_sink_pad);

    self->tee_sink_pad = tee_sink_pad;
    gst_element_add_pad(self, self->sinkpad);

    src_queue_src_pad = gst_element_get_static_pad(self->src_queue, "src");
    self->srcpad = gst_ghost_pad_new("src", src_queue_src_pad);

    gst_element_add_pad(self, self->srcpad);

    tee_src_pad = gst_element_get_request_pad(self->tee, "src_%u");
    self->tee_src_pad = tee_src_pad;
    gst_pad_set_active(tee_src_pad, TRUE);
    tee_dump_pad = gst_element_get_request_pad(self->tee, "src_%u");
    self->tee_dump_pad = tee_dump_pad;
    gst_pad_set_active(tee_dump_pad, TRUE);

    src_queue_sink_pad = gst_element_get_static_pad(self->src_queue, "sink");
    gst_pad_link(tee_src_pad, src_queue_sink_pad);

    dump_queue_sink_pad = gst_element_get_static_pad(self->dump_queue, "sink");
    gst_pad_link(tee_dump_pad, dump_queue_sink_pad);

    dump_queue_src_pad = gst_element_get_static_pad(self->dump_queue, "src");
    appsink_sink_pad = gst_element_get_static_pad(self->appsink, "sink");
    gst_pad_link(dump_queue_src_pad, appsink_sink_pad);

    gst_pad_add_probe(appsink_sink_pad, GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM, (GstPadProbeCallback)appsink_sink_pad_probe_cb, self, NULL);

    g_object_set (self->appsink, "emit-signals", TRUE, NULL);
    g_signal_connect (self->appsink, "new-sample", G_CALLBACK (new_sample), self);
    g_signal_connect(self->appsink, "eos", G_CALLBACK(received_eos), self);

    gst_object_unref(tee_sink_pad);
    gst_object_unref(tee_src_pad);
    gst_object_unref(tee_dump_pad);
    gst_object_unref(src_queue_sink_pad);
    gst_object_unref(src_queue_src_pad);
    gst_object_unref(dump_queue_sink_pad);
    gst_object_unref(dump_queue_src_pad);
    gst_object_unref(appsink_sink_pad);

    //debug
    strcpy(self->dump_dir, "/home/whr");
    strcpy(self->source_name, "dump_test");
}

static void gst_dump_helper_dispose(GObject *object)
{
    GstDumpHelper *self = GST_DUMP_HELPER(object);

    G_OBJECT_CLASS(parent_class)->dispose(object);
}

static void
gst_dump_helper_finalize (GObject * object)
{
    GstDumpHelper *self = GST_DUMP_HELPER(object);

    if (self->dump_stream) {
        close_dump_stream(self->dump_stream);

        release_dump_stream(self->dump_stream);
        self->dump_stream = NULL;
    }

    G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void gst_dump_helper_set_property(GObject *object, guint prop_id,
    const GValue *value, GParamSpec *pspec)
{
    switch (prop_id) {

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void gst_dump_helper_get_property(GObject *object, guint prop_id,
    GValue *value, GParamSpec *pspec)
{
    GstDumpHelper *self = GST_DUMP_HELPER(object);

    switch (prop_id) {

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static GstStateChangeReturn gst_dump_heleper_change_state(GstElement *element, GstStateChange transition)
{
    GstDumpHelper *self = GST_DUMP_HELPER(element);
    GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;

//    GST_DEBUG_OBJECT(element, "dumphelper chage_state in:%s to %s",
//            gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
//            gst_element_state_get_name((GstState)(transition & 0x07)));

    switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
        break;

    case GST_STATE_CHANGE_READY_TO_PAUSED:
    {


//        g_print("begin open:\n");
//        g_fp = fopen("/home/whr/dump.mp3", "wb");
//        if (g_fp == NULL) {
//            g_print("open g_fp failed! %s\n", strerror(errno));
//        }
    }
        break;

    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
    {
        GstCaps *caps;

        g_object_get(self->tee_sink_pad, "caps", &caps, NULL);
        gchar *caps_string;
        caps_string = gst_caps_to_string(caps);
   //     GST_INFO("PAUSED state caps:%s", caps_string);
    }
        break;
    }

    ret = GST_ELEMENT_CLASS(parent_class)->change_state(element, transition);

    switch (transition) {
    case GST_STATE_CHANGE_PLAYING_TO_PAUSED:

        break;

    case GST_STATE_CHANGE_PAUSED_TO_READY:
        break;

    case GST_STATE_CHANGE_READY_TO_NULL:
        break;
    }

    return ret;
}

gboolean dump_helper_plugin_init (GstPlugin * plugin)
{
  GST_DEBUG_CATEGORY_INIT (gst_dump_helper_debug, "dumphelper",
      0, "dump helper");

  return gst_element_register (plugin, "dumphelper", GST_RANK_NONE,
      GST_TYPE_DUMP_HELPER);
}





