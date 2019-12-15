#include <gst/gst.h>

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *sink;
    GstElement *video_convert;
    GstElement *video_sink;
}CustomData;

static void pad_add_handler(GstElement *src, GstPad* pad, CustomData *data);


int main(int argc, char *argv[])
{
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;


//    setenv("GST_DEBUG", "*:DEBUG", 1);


    gst_init(&argc, &argv);

    data.source = gst_element_factory_make("uridecodebin", "source");
    data.convert = gst_element_factory_make("audioconvert", "convert");
    data.sink = gst_element_factory_make("autoaudiosink", "sink");
    data.video_convert = gst_element_factory_make("videoconvert", "video_convert");
    data.video_sink = gst_element_factory_make("autovideosink", "video_sink");

    data.pipeline = gst_pipeline_new("test_pipeline");

    if (!data.pipeline || !data.source || !data.sink || !data.video_convert || !data.video_sink) {
        g_printerr("Not all elements could be createed.\n");
        return -1;
    }

    gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.convert, data.sink, data.video_convert, data.video_sink, NULL);

    if (!gst_element_link(data.convert, data.sink)) {
        g_printerr("Elements could not be linked!.\n");
        g_object_unref(data.pipeline);
        return -1;
    }

    if (!gst_element_link(data.video_convert, data.video_sink)) {
        g_printerr("Elements could not be linked!.\n");
        g_object_unref(data.pipeline);
        return -1;
    }

    g_print("video_convert name:%s\n", gst_element_get_name(data.video_convert));

    g_object_set(data.source, "uri", "file:///home/whr/workspace/assets/mkv/sintel_trailer-480p.webm", NULL);

    g_signal_connect(data.source, "pad-added", G_CALLBACK(pad_add_handler), &data);

    ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state\n");
        g_object_unref(data.pipeline);
        return -1;
    }

    bus = gst_element_get_bus(data.pipeline);
    do {
        msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                         GST_MESSAGE_STATE_CHANGED|GST_MESSAGE_ERROR|GST_MESSAGE_EOS);
        if (msg != NULL) {
            GError *err;
            gchar *debug_info;

            switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s:%s\n", msg->src, err->message);
                g_printerr("Debugging information: %s\n", debug_info);
                g_clear_error(err);
                g_free(debug_info);
                terminate = TRUE;
                break;

            case GST_MESSAGE_STATE_CHANGED:
                if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data.pipeline)) {
                    GstState old_state, new_state, pending_state;
                    gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
                    g_printerr("Pipeline state changed from %s to %s\n", gst_element_state_get_name(old_state),
                               gst_element_state_get_name(new_state));
                }
                break;

            case GST_MESSAGE_EOS:
                g_printerr("End-of-stream reached\n");
                break;

            default:
                g_printerr("Unexpected message received.\n");
                break;
            }
        }
    } while (!terminate);

    return 0;
}

static void pad_add_handler(GstElement *src, GstPad *new_pad, CustomData *data)
{
    GstPad *sink_pad = gst_element_get_static_pad(data->convert, "sink");
    GstPad *video_sink_pad = gst_element_get_static_pad(data->video_convert, "sink");
    GstPad *pad;
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;

    g_print("Received new pad '%s' from %s:\n", GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src));


    new_pad_caps = gst_pad_query_caps(new_pad, NULL);
    new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    new_pad_type = gst_structure_get_name(new_pad_struct);
    g_print("%s\n", gst_caps_to_string(new_pad_caps));

    if (g_str_has_prefix(new_pad_type, "audio/x-raw")) {
        if (gst_pad_is_linked(sink_pad)) {
            g_print("We are already linked. Ignoreing.\n");
            goto exit;
        }
        pad = sink_pad;
    } else if (g_str_has_prefix(new_pad_type, "video/x-raw")) {
        if (gst_pad_is_linked(video_sink_pad)) {
            g_print("We are already linked. Ignoreing.\n");
            goto exit;
        }
        pad = video_sink_pad;
    } else {
        goto exit;
    }
    GstCaps* pad_cpas = gst_pad_query_caps(pad, NULL);
    g_print("%s\n", gst_caps_to_string(pad_cpas));
    ret = gst_pad_link(new_pad, pad);
    if (GST_PAD_LINK_FAILED(ret)) {
        g_print("Type is '%s' but link failed: %d\n", new_pad_type, ret);
    } else {
        g_print("Link successed (type '%s')\n", new_pad_type);
    }

exit:
    if (new_pad_caps != NULL) {
        gst_caps_unref(new_pad_caps);
    }

    gst_object_unref(sink_pad);
    gst_object_unref(video_sink_pad);
}
