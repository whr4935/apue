#include <string.h>
#include "main.h"

////////////////////////////////
CustomData g_data;

int main(int argc, char *argv[]) {

    /* Initialize GTK */
    gtk_init (&argc, &argv);

    /* Initialize GStreamer */
    gst_init (&argc, &argv);

    /* Initialize our data structure */
    memset (&g_data, 0, sizeof (g_data));
    g_data.duration = GST_CLOCK_TIME_NONE;

    init_pipeline();

    /* Create the GUI */
    create_ui (&g_data);

    /* Register a function that GLib will call every second */
    g_timeout_add_seconds (1, (GSourceFunc)refresh_ui, &g_data);

    /* Start the GTK main loop. We will not regain control until gtk_main_quit is called. */
    gtk_main ();

    deinit_pipeline();

    return 0;
}
