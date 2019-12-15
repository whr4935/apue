TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    gst.c \
    ui.c

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-video-1.0 gtk+-2.0

HEADERS += \
    main.h


