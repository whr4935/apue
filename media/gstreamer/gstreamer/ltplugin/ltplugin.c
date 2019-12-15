#include "ltplugin.h"



static gboolean plugin_init(GstPlugin *plugin)
{
    gboolean res;

    res = dump_helper_plugin_init(plugin);


    return res;
}


#ifndef PACKAGE
#define PACKAGE "lt plugins"
#endif

#define VERSION "1.0"

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    ltplugin,
    "lt plugins(multimedia)",
    plugin_init,
    VERSION,
    "LGPL",
    "GStreamer",
    "http://www.china-liantong.com"
)
