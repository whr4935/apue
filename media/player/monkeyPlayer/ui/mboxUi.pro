
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mbox
TEMPLATE = app

SOURCES += main.cpp\
    ui/mainwindow.cpp \
    core/MessageQueue.cpp \
    source/ScreenGrab.cpp \
    renderer/OpenGLRenderer.cpp \
    renderer/Renderer.cpp \
    encode/H264Encode.cpp \
    streaming/RTMPStreaming.cpp \
    core/Frame.cpp \
    source/Source.cpp

HEADERS  += ui/mainwindow.h \
    include/MessageQueue.h \
    source/ScreenGrab.h \
    renderer/Renderer.h \
    renderer/OpenGLRenderer.h \
    encode/H264Encode.h \
    streaming/RTMPStreaming.h \
    include/Frame.h \
    include/includes.h \
    include/Source.h

FORMS    += ui/mainwindow.ui



DISTFILES += \
    renderer/yuv420.frag \
    renderer/yuv420.vert


DEFINES +=

INCLUDEPATH += include

LIBS += -lX11 -lpthread -lx264 -L../../lib
QMAKE_LFLAGS_APP += -Wl,-rpath=$$(BUILD_DIR)"/lib"
DESTDIR = ../..

CONFIG += debug



