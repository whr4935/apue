TEMPLATE = lib

QT       += core gui widgets

TARGET = iconeditorplugin

SOURCES = \
    $$PWD/iconeditor.cpp \
    $$PWD/iconeditorplugin.cpp

DISTFILES += \
    $$PWD/images/mouse.png \
    $$PWD/iconeditorplugin.json

RESOURCES += \
    $$PWD/iconeditor.qrc

HEADERS += \
    $$PWD/iconeditor.h \
    $$PWD/iconeditorplugin.h
