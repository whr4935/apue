#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

#include <gdk/gdk.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

#include "main.h"


//////////////////////////////////////////
// 打开文件
static void file_open(GtkAction *action)
{
    GtkWidget *file_chooser = gtk_file_chooser_dialog_new(
                "Open File", GTK_WINDOW(g_data.main_window),
                GTK_FILE_CHOOSER_ACTION_OPEN,
                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                NULL);

    gtk_file_chooser_set_current_folder(file_chooser, "/home/whr/workspace/assets/");

    if (gtk_dialog_run(GTK_DIALOG(file_chooser)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(file_chooser));
        g_signal_emit_by_name(G_OBJECT(g_data.stop_button), "clicked");

        if (g_data.filename)
            g_free(g_data.filename);
        g_data.filename = filename;

        if (load_file(g_data.filename))
            gtk_widget_set_sensitive(GTK_WIDGET(g_data.play_button), TRUE);
    }

    gtk_widget_destroy(file_chooser);
}

// 关于
static void help_about(GtkAction *action)
{
    GtkWidget *about_dialog = gtk_about_dialog_new();
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about_dialog), "MediaPlayer");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_dialog), "1.0.0");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about_dialog), "Copyright @ 2011, Figo");

    gtk_dialog_run(GTK_DIALOG(about_dialog));
    gtk_widget_destroy(about_dialog);
}

static GtkActionEntry mainwindow_action_entries[] = {
    { "FileMenu", "NULL", "文件" },
    {
        "OpenFile",
        GTK_STOCK_OPEN,
        "打开(O)",
        "<control>O",
        "Open a file for playback",
        G_CALLBACK(file_open)
    },
    {
        "QuitPlayer",
        GTK_STOCK_QUIT,
        "退出(Q)",
        "<control>Q",
        "Quit the media player",
        G_CALLBACK(delete_event_cb)
    },

    { "HelpMenu", "NULL", "帮助" },
    {
        "HelpAbout",
        GTK_STOCK_ABOUT,
        "关于",
        "",
        "About the media player",
        G_CALLBACK(help_about)
    }
};

////////////////////////////////////////////////////
/* This function is called when the GUI toolkit creates the physical window that will hold the video.
 * At this point we can retrieve its handler (which has a different meaning depending on the windowing system)
 * and pass it to GStreamer through the GstVideoOverlay interface. */
static void realize_cb (GtkWidget *widget, CustomData *data)
{
    GdkWindow *window = gtk_widget_get_window (widget);
    guintptr window_handle;

    if (!gdk_window_ensure_native (window))
        g_error ("Couldn't create native window needed for GstVideoOverlay!");

    /* Retrieve window handler from GDK */
#if defined (GDK_WINDOWING_WIN32)
    window_handle = (guintptr)GDK_WINDOW_HWND (window);
#elif defined (GDK_WINDOWING_QUARTZ)
    window_handle = gdk_quartz_window_get_nsview (window);
#elif defined (GDK_WINDOWING_X11)
    window_handle = GDK_WINDOW_XID (window);
#endif
    /* Pass it to playbin, which implements GstVideoOverlay and will forward it to the video sink */
    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (data->playbin), window_handle);
}

/* This function is called when the PLAY button is clicked */
static void play_cb (GtkButton *button, CustomData *data)
{
    if (data->filename == NULL)
        return file_open(NULL);

    gst_element_set_state (data->playbin, GST_STATE_PLAYING);
}

/* This function is called when the PAUSE button is clicked */
static void pause_cb (GtkButton *button, CustomData *data)
{
    gst_element_set_state (data->playbin, GST_STATE_PAUSED);
}

/* This function is called when the STOP button is clicked */
static void stop_cb (GtkButton *button, CustomData *data)
{
    GstStateChangeReturn ret =  gst_element_set_state (data->playbin, GST_STATE_READY);

    GstState state;
    ret = gst_element_get_state(data->playbin, &state, NULL, GST_CLOCK_TIME_NONE);
    if (ret == GST_STATE_CHANGE_SUCCESS)
        data->state = state;

}

/* This function is called when the main window is closed */
void delete_event_cb (GtkWidget *widget, GdkEvent *event, CustomData *data)
{
    stop_cb (NULL, data);
    gtk_main_quit ();
}

/* This function is called everytime the video window needs to be redrawn (due to damage/exposure,
 * rescaling, etc). GStreamer takes care of this in the PAUSED and PLAYING states, otherwise,
 * we simply draw a black rectangle to avoid garbage showing up. */
static gboolean draw_cb (GtkWidget *widget, cairo_t *cr, CustomData *data)
{
    if (data->state < GST_STATE_PAUSED) {
        GtkAllocation allocation;

        /* Cairo is a 2D graphics library which we use here to clean the video window.
     * It is used by GStreamer for other reasons, so it will always be available to us. */
        gtk_widget_get_allocation (widget, &allocation);
        cairo_set_source_rgb (cr, 0, 0, 0);
        cairo_rectangle (cr, 0, 0, allocation.width, allocation.height);
        cairo_fill (cr);
        //    cairo_destroy (cr);
    }

    return FALSE;
}

/* This function is called when the slider changes its position. We perform a seek to the
 * new position here. */
static void slider_cb (GtkRange *range, CustomData *data)
{
    gdouble value = gtk_range_get_value (GTK_RANGE (data->slider));
    gst_element_seek_simple (data->playbin, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT,
                             (gint64)(value * GST_SECOND));
}

/* This function is called periodically to refresh the GUI */
gboolean refresh_ui (CustomData *data)
{
    gint64 current = -1;

    /* We do not want to update anything unless we are in the PAUSED or PLAYING states */
    if (data->state < GST_STATE_PAUSED)
        return TRUE;

    /* If we didn't know it yet, query the stream duration */
    if (!GST_CLOCK_TIME_IS_VALID (data->duration)) {
        if (!gst_element_query_duration (data->playbin, GST_FORMAT_TIME, &data->duration)) {
            g_printerr ("Could not query current duration.\n");
        } else {
            /* Set the range of the slider to the clip duration, in SECONDS */
            gtk_range_set_range (GTK_RANGE (data->slider), 0, (gdouble)data->duration / GST_SECOND);

            gchar time_buffer[24];
            g_snprintf(time_buffer, 24, "%u:%02u:%02u", GST_TIME_ARGS(data->duration));
            gtk_label_set_text(GTK_LABEL(data->duration_label), time_buffer);
        }
    }

    if (gst_element_query_position (data->playbin, GST_FORMAT_TIME, &current)) {
        /* Block the "value-changed" signal, so the slider_cb function is not called
     * (which would trigger a seek the user has not requested) */
        g_signal_handler_block (data->slider, data->slider_update_signal_id);
        /* Set the position of the slider to the current pipeline positoin, in SECONDS */
        gtk_range_set_value (GTK_RANGE (data->slider), (gdouble)current / GST_SECOND);
        /* Re-enable the signal */
        g_signal_handler_unblock (data->slider, data->slider_update_signal_id);

        gchar time_buffer[24];
        g_snprintf(time_buffer, 24, "%u:%02u:%02u", GST_TIME_ARGS(current));
        gtk_label_set_text(GTK_LABEL(data->time_label), time_buffer);
    }

    return TRUE;
}

/* This creates all the GTK+ widgets that compose our application, and registers the callbacks */
void create_ui (CustomData *data)
{
    GtkWidget *main_box;     /* VBox to hold main_hbox and the controls */
    GtkWidget *main_hbox;    /* HBox to hold the video_window and the stream info text widget */
    GtkWidget *controls;     /* HBox to hold the buttons and the slider */

    data->main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (data->main_window), "delete-event", G_CALLBACK (delete_event_cb), data);

    data->video_window = gtk_drawing_area_new ();
    gtk_widget_set_double_buffered (data->video_window, FALSE);
    g_signal_connect (data->video_window, "realize", G_CALLBACK (realize_cb), data);
    g_signal_connect (data->video_window, "draw", G_CALLBACK (draw_cb), data);

    data->play_button = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
    g_signal_connect (G_OBJECT (data->play_button), "clicked", G_CALLBACK (play_cb), data);

    data->pause_button = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PAUSE);
    g_signal_connect (G_OBJECT (data->pause_button), "clicked", G_CALLBACK (pause_cb), data);

    data->stop_button = gtk_button_new_from_stock (GTK_STOCK_MEDIA_STOP);
    g_signal_connect (G_OBJECT (data->stop_button), "clicked", G_CALLBACK (stop_cb), data);

    data->slider = gtk_hscale_new_with_range (0, 100, 1);
    gtk_scale_set_draw_value (GTK_SCALE (data->slider), 0);
    data->slider_update_signal_id = g_signal_connect (G_OBJECT (data->slider), "value-changed", G_CALLBACK (slider_cb), data);

    data->time_label = gtk_label_new("00:00:00");
    gtk_misc_set_alignment(GTK_MISC(data->time_label), 0.5, 0.5);

    data->duration_label = gtk_label_new("00:00:00");
    gtk_misc_set_alignment(GTK_MISC(data->duration_label), 0.5, 0.5);

    data->streams_list = gtk_text_view_new ();
    gtk_text_view_set_editable (GTK_TEXT_VIEW (data->streams_list), FALSE);

    controls = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (controls), data->play_button, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (controls), data->pause_button, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (controls), data->stop_button, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(controls), data->time_label, FALSE, TRUE, 2);
    gtk_box_pack_start (GTK_BOX (controls), data->slider, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(controls), data->duration_label, FALSE, TRUE, 2);


    main_hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (main_hbox), data->video_window, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (main_hbox), data->streams_list, FALSE, FALSE, 2);

    main_box = gtk_vbox_new (FALSE, 0);

    // 添加菜单栏
    GtkActionGroup *actiongroup;
    GtkUIManager *ui_manager;

    actiongroup = gtk_action_group_new("MainwindowActiongroup");
    gtk_action_group_add_actions(actiongroup,
                                 mainwindow_action_entries,
                                 G_N_ELEMENTS(mainwindow_action_entries),
                                 NULL);
    ui_manager = gtk_ui_manager_new();
    gtk_ui_manager_insert_action_group(ui_manager, actiongroup, 0);
    gtk_ui_manager_add_ui_from_string(
                ui_manager,
                "<ui>"
                "    <menubar name='MainMenu'>"
                "        <menu action='FileMenu'>"
                "            <menuitem action='OpenFile'/>"
                "            <separator name='fsep1'/>"
                "            <menuitem action='QuitPlayer'/>"
                "        </menu>"
                "        <menu action='HelpMenu'>"
                "            <menuitem action='HelpAbout'/>"
                "        </menu>"
                "    </menubar>"
                "</ui>",
                -1,
                NULL);

    gtk_box_pack_start(
                GTK_BOX(main_box),
                gtk_ui_manager_get_widget(ui_manager, "/ui/MainMenu"),
                FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (main_box), main_hbox, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (main_box), controls, FALSE, FALSE, 0);
    gtk_container_add (GTK_CONTAINER (data->main_window), main_box);
    gtk_window_set_default_size (GTK_WINDOW (data->main_window), 640, 480);

    gtk_widget_show_all (data->main_window);
}
