#include "main.h"


/* Functions below print the Capabilities in a human-friendly format */
static gboolean print_field (GQuark field, const GValue * value, gpointer pfx)
{
  gchar *str = gst_value_serialize (value);

  g_print ("%s  %15s: %s\n", (gchar *) pfx, g_quark_to_string (field), str);
  g_free (str);
  return TRUE;
}

static void print_caps (const GstCaps * caps, const gchar * pfx)
{
  guint i;

  g_return_if_fail (caps != NULL);

  if (gst_caps_is_any (caps)) {
    g_print ("%sANY\n", pfx);
    return;
  }
  if (gst_caps_is_empty (caps)) {
    g_print ("%sEMPTY\n", pfx);
    return;
  }

  for (i = 0; i < gst_caps_get_size (caps); i++) {
    GstStructure *structure = gst_caps_get_structure (caps, i);

    g_print ("%s%s\n", pfx, gst_structure_get_name (structure));
    gst_structure_foreach (structure, print_field, (gpointer) pfx);
  }
}

/* Prints information about a Pad Template, including its Capabilities */
static void print_pad_templates_information (GstElementFactory * factory)
{
  const GList *pads;
  GstStaticPadTemplate *padtemplate;

  g_print ("Pad Templates for %s:\n", gst_element_factory_get_longname (factory));
  if (!gst_element_factory_get_num_pad_templates (factory)) {
    g_print ("  none\n");
    return;
  }

  pads = gst_element_factory_get_static_pad_templates (factory);
  while (pads) {
    padtemplate = pads->data;
    pads = g_list_next (pads);

    if (padtemplate->direction == GST_PAD_SRC)
      g_print ("  SRC template: '%s'\n", padtemplate->name_template);
    else if (padtemplate->direction == GST_PAD_SINK)
      g_print ("  SINK template: '%s'\n", padtemplate->name_template);
    else
      g_print ("  UNKNOWN!!! template: '%s'\n", padtemplate->name_template);

    if (padtemplate->presence == GST_PAD_ALWAYS)
      g_print ("    Availability: Always\n");
    else if (padtemplate->presence == GST_PAD_SOMETIMES)
      g_print ("    Availability: Sometimes\n");
    else if (padtemplate->presence == GST_PAD_REQUEST) {
      g_print ("    Availability: On request\n");
    } else
      g_print ("    Availability: UNKNOWN!!!\n");

    if (padtemplate->static_caps.string) {
      GstCaps *caps;
      g_print ("    Capabilities:\n");
      caps = gst_static_caps_get (&padtemplate->static_caps);
      print_caps (caps, "      ");
      gst_caps_unref (caps);

    }

    g_print ("\n");
  }
}

/* Shows the CURRENT capabilities of the requested pad in the given element */
static void print_pad_capabilities (GstElement *element, gchar *pad_name)
{
  GstPad *pad = NULL;
  GstCaps *caps = NULL;

  /* Retrieve pad */
  pad = gst_element_get_static_pad (element, pad_name);
  if (!pad) {
    g_printerr ("Could not retrieve pad '%s'\n", pad_name);
    return;
  }

  /* Retrieve negotiated caps (or acceptable caps if negotiation is not finished yet) */
  caps = gst_pad_get_current_caps (pad);
  if (!caps)
    caps = gst_pad_query_caps (pad, NULL);

  /* Print and free */
  g_print ("Caps for the %s pad:\n", pad_name);
  print_caps (caps, "      ");
  gst_caps_unref (caps);
  gst_object_unref (pad);
}

///////////////////////////////////////////////////////////////////////////
/* This function is called when new metadata is discovered in the stream */
void tags_cb (GstElement *playbin, gint stream, CustomData *data)
{
    /* We are possibly in a GStreamer working thread, so we notify the main
   * thread of this event through a message in the bus */
    gst_element_post_message (playbin,
                              gst_message_new_application (GST_OBJECT (playbin),
                                                           gst_structure_new ("tags-changed", NULL)));
}

/* This function is called when an error message is posted on the bus */
static void error_cb (GstBus *bus, GstMessage *msg, CustomData *data)
{
    GError *err;
    gchar *debug_info;

    /* Print error details on the screen */
    gst_message_parse_error (msg, &err, &debug_info);
    g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
    g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
    g_clear_error (&err);
    g_free (debug_info);

    /* Set the pipeline to READY (which stops playback) */
    gst_element_set_state (data->playbin, GST_STATE_READY);
}

/* This function is called when an End-Of-Stream message is posted on the bus.
 * We just set the pipeline to READY (which stops playback) */
static void eos_cb (GstBus *bus, GstMessage *msg, CustomData *data)
{
    g_print ("End-Of-Stream reached.\n");
    gst_element_set_state (data->playbin, GST_STATE_READY);
}

/* This function is called when the pipeline changes states. We use it to
 * keep track of the current state. */
static void state_changed_cb (GstBus *bus, GstMessage *msg, CustomData *data)
{
    GstState old_state, new_state, pending_state;
    gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
    if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data->playbin)) {
        data->state = new_state;
        g_print ("State set to %s\n", gst_element_state_get_name (new_state));
        if (old_state == GST_STATE_READY && new_state == GST_STATE_PAUSED) {
            /* For extra responsiveness, we refresh the GUI as soon as we reach the PAUSED state */
            refresh_ui (data);
        }
    }
}

/* Extract metadata from all the streams and write it to the text widget in the GUI */
static void analyze_streams (CustomData *data)
{
    gint i;
    GstTagList *tags;
    gchar *str, *total_str;
    guint rate;
    gint n_video, n_audio, n_text;
    GtkTextBuffer *text;

    /* Clean current contents of the widget */
    text = gtk_text_view_get_buffer (GTK_TEXT_VIEW (data->streams_list));
    gtk_text_buffer_set_text (text, "", -1);

    /* Read some properties */
    g_object_get (data->playbin, "n-video", &n_video, NULL);
    g_object_get (data->playbin, "n-audio", &n_audio, NULL);
    g_object_get (data->playbin, "n-text", &n_text, NULL);

    for (i = 0; i < n_video; i++) {
        tags = NULL;
        /* Retrieve the stream's video tags */
        g_signal_emit_by_name (data->playbin, "get-video-tags", i, &tags);
        if (tags) {
            total_str = g_strdup_printf ("video stream %d:\n", i);
            gtk_text_buffer_insert_at_cursor (text, total_str, -1);
            g_free (total_str);
            gst_tag_list_get_string (tags, GST_TAG_VIDEO_CODEC, &str);
            total_str = g_strdup_printf ("  codec: %s\n", str ? str : "unknown");
            gtk_text_buffer_insert_at_cursor (text, total_str, -1);
            g_free (total_str);
            g_free (str);
            gst_tag_list_free (tags);
        }
    }

    for (i = 0; i < n_audio; i++) {
        tags = NULL;
        /* Retrieve the stream's audio tags */
        g_signal_emit_by_name (data->playbin, "get-audio-tags", i, &tags);
        if (tags) {
            total_str = g_strdup_printf ("\naudio stream %d:\n", i);
            gtk_text_buffer_insert_at_cursor (text, total_str, -1);
            g_free (total_str);
            if (gst_tag_list_get_string (tags, GST_TAG_AUDIO_CODEC, &str)) {
                total_str = g_strdup_printf ("  codec: %s\n", str);
                gtk_text_buffer_insert_at_cursor (text, total_str, -1);
                g_free (total_str);
                g_free (str);
            }
            if (gst_tag_list_get_string (tags, GST_TAG_LANGUAGE_CODE, &str)) {
                total_str = g_strdup_printf ("  language: %s\n", str);
                gtk_text_buffer_insert_at_cursor (text, total_str, -1);
                g_free (total_str);
                g_free (str);
            }
            if (gst_tag_list_get_uint (tags, GST_TAG_BITRATE, &rate)) {
                total_str = g_strdup_printf ("  bitrate: %d\n", rate);
                gtk_text_buffer_insert_at_cursor (text, total_str, -1);
                g_free (total_str);
            }
            gst_tag_list_free (tags);
        }
    }

    for (i = 0; i < n_text; i++) {
        tags = NULL;
        /* Retrieve the stream's subtitle tags */
        g_signal_emit_by_name (data->playbin, "get-text-tags", i, &tags);
        if (tags) {
            total_str = g_strdup_printf ("\nsubtitle stream %d:\n", i);
            gtk_text_buffer_insert_at_cursor (text, total_str, -1);
            g_free (total_str);
            if (gst_tag_list_get_string (tags, GST_TAG_LANGUAGE_CODE, &str)) {
                total_str = g_strdup_printf ("  language: %s\n", str);
                gtk_text_buffer_insert_at_cursor (text, total_str, -1);
                g_free (total_str);
                g_free (str);
            }
            gst_tag_list_free (tags);
        }
    }
}

/* This function is called when an "application" message is posted on the bus.
 * Here we retrieve the message posted by the tags_cb callback */
static void application_cb (GstBus *bus, GstMessage *msg, CustomData *data)
{
    if (g_strcmp0 (gst_structure_get_name (gst_message_get_structure(msg)), "tags-changed") == 0) {
        /* If the message is the "tags-changed" (only one we are currently issuing), update
     * the stream info GUI */
        analyze_streams (data);
    }
}

static gboolean build_gstreamer_pipeline(const gchar *uri)
{
    g_object_set (G_OBJECT (g_data.playbin), "uri", uri, NULL);

#if 1
    /* Start playing */
    GstStateChangeReturn ret = gst_element_set_state (g_data.playbin, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (g_data.playbin);
        return -1;
    }
#endif

    return TRUE;
}
// load file to play
gboolean load_file(const gchar *uri)
{
    if (build_gstreamer_pipeline(uri))
        return TRUE;
    return FALSE;
}

gboolean init_pipeline()
{
    g_data.playbin = gst_element_factory_make ("playbin", "playbin");

    if (!g_data.playbin) {
      g_printerr ("Not all elements could be created.\n");
      return FALSE;
    }

    GstElement *sink = gst_element_factory_make("autovideosink", "sink");
    print_pad_capabilities(sink, "sink");
    gst_object_unref(sink);

    /* Connect to interesting signals in playbin */
    g_signal_connect (G_OBJECT (g_data.playbin), "video-tags-changed", (GCallback) tags_cb, &g_data);
    g_signal_connect (G_OBJECT (g_data.playbin), "audio-tags-changed", (GCallback) tags_cb, &g_data);
    g_signal_connect (G_OBJECT (g_data.playbin), "text-tags-changed", (GCallback) tags_cb, &g_data);

    /* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
    GstBus *bus = gst_element_get_bus (g_data.playbin);
    gst_bus_add_signal_watch (bus);
    g_signal_connect (G_OBJECT (bus), "message::error", (GCallback)error_cb, &g_data);
    g_signal_connect (G_OBJECT (bus), "message::eos", (GCallback)eos_cb, &g_data);
    g_signal_connect (G_OBJECT (bus), "message::state-changed", (GCallback)state_changed_cb, &g_data);
    g_signal_connect (G_OBJECT (bus), "message::application", (GCallback)application_cb, &g_data);
    gst_object_unref (bus);

    return TRUE;
}

gboolean deinit_pipeline()
{
    /* Free resources */
    gst_element_set_state (g_data.playbin, GST_STATE_NULL);
    gst_object_unref (g_data.playbin);

    return TRUE;
}
