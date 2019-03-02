TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    echoserver.cpp

include($$PWD/../unp.pri)

HEADERS += \
    echoserver.h
