TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    EchoClient.cpp

include($$PWD/../unp.pri)

HEADERS += \
    EchoClient.h
