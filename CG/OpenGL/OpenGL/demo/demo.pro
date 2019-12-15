TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH +=../include
DEFINES += _LINUX

CONFIG += link_pkgconfig
PKGCONFIG += gl x11 xxf86vm

LIBS+= -L$$PWD/../lib -lsb6 -lglfw -lpthread -ldl

DESTDIR = ../bin
