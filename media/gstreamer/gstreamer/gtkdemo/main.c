#include <gtk/gtk.h>

gboolean delete_event(GtkWidget* widget, GdkEvent* event, gpointer* data)
{
    g_print("hello world!\n");

    return FALSE;
}

gboolean hello(GtkWidget* widget, GdkEvent* event, gpointer* data)
{
    static  guint count;
    g_print("hello: %d\n", ++count);

    return FALSE;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(window);

    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    GtkWidget* button;

    button = gtk_button_new_with_label("Hello");
   // g_signal_connect(button, "clicked", G_CALLBACK(hello), NULL);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    g_signal_connect_swapped(window, "destroy", (gtk_main_quit), NULL);
    gtk_container_add(GTK_CONTAINER(window), button);
    gtk_widget_show(button);

    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);

    gtk_main();


    printf("Hello World!\n");
    return 0;
}
