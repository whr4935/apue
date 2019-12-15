#ifndef MAIN_H
#define MAIN_H

#include <gst/gst.h>
#include <gtk/gtk.h>

/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData {
    GtkWidget *slider;              /* Slider widget to keep track of current position */
    GtkWidget *time_label;
    GtkWidget *duration_label;
    GtkWidget *streams_list;        /* Text widget to display info about the streams */
    gulong slider_update_signal_id; /* Signal ID for the slider update signal */
    GtkWidget *main_window;
    GtkWidget *video_window; /* The drawing area where the video will be shown */
    GtkWidget *play_button;
    GtkWidget *pause_button;
    GtkWidget *stop_button;

    GstElement *playbin;           /* Our one and only pipeline */
    GstState state;                 /* Current state of the pipeline */
    gint64 duration;                /* Duration of the clip, in nanoseconds */

    gchar* filename;
} CustomData;

extern CustomData g_data;



void gui_update_time(const gchar *time, const gint64 position, const gint64 length);
// void gui_update_metadata(const gchar *title, const gchar *artist);


gboolean load_file(const gchar *uri);
void seek_to(gdouble percentage);

gboolean init_pipeline();
gboolean deinit_pipeline();


gboolean refresh_ui (CustomData *data);
void delete_event_cb (GtkWidget *widget, GdkEvent *event, CustomData *data);

void create_ui (CustomData *data) ;

#endif // MAIN_H
