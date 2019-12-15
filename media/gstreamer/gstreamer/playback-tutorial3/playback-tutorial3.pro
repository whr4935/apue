TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0 gstreamer-audio-1.0

SOURCES += main.c
