TEMPLATE = lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
TARGET = gstltplugin

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0

SOURCES += \
    dumphelper.c \
    ltplugin.c

HEADERS += \
    ltplugin.h \
    dumpstreamtype.h


copydata.commands = $(COPY_DIR) $$OUT_PWD/$(TARGET) $$OUT_PWD/libgstltplugin.so  /home/whr/gstplugins
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
