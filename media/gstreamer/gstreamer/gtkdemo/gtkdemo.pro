TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c


CONFIG += link_pkgconfig
PKGCONFIG += gtk+-2.0
