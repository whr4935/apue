QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE      = lib
CONFIG       += plugin
HEADERS       = ../textart/textartinterface.h \
                basiceffectsplugin.h
SOURCES       = basiceffectsplugin.cpp
DESTDIR       = ../textart/plugins
