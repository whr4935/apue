#-------------------------------------------------
#
# Project created by QtCreator 2017-08-11T23:38:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spreadsheet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    finddialog.cpp \
    gotocelldialog.cpp \
    sortdialog.cpp \
    spreadsheet.cpp


HEADERS  += mainwindow.h \
    SortDialog.h \
    cell.h \
    finddialog.h \
    gotocelldialog.h \
    sortdialog.h \
    spreadsheet.h

RESOURCES += \
    spreadsheet.qrc

