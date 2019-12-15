TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += src/include src/modules
DEFINES +=
LIBS += -pthread

SOURCES += \
    src/main.cpp \
    src/SilentDream.cpp \
    src/ArgumentParser.cpp \
    src/Log.cpp \
    src/Epoll.cpp \
    src/Socket.cpp

HEADERS += \
    src/ArgumentParser.h \
    src/Epoll.h \
    src/Socket.h
