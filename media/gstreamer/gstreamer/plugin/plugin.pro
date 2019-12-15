TEMPLATE = lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
TARGET = myplugin

SOURCES += \
    myplugin.c

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0

HEADERS += \
    myplugin.h

copydata.commands = $(COPY_DIR) $$OUT_PWD/$(TARGET) $$OUT_PWD/libmyplugin.so  /home/whr/gstplugins
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
