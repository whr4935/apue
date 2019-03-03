TEMPLATE = app

QT       += core gui widgets

TARGET = qtlearn

HEADERS += \
    $$PWD/qtlearn.h \
    $$PWD/360safe/dropshadowwidget.h \
    $$PWD/360safe/mainwidget.h \
    $$PWD/360safe/push_button.h \
    $$PWD/360safe/titlewidget.h \
    $$PWD/360safe/tool_button.h 

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/qtlearn.cpp \
    $$PWD/360safe/dropshadowwidget.cpp \
    $$PWD/360safe/mainwidget.cpp \
    $$PWD/360safe/push_button.cpp \
    $$PWD/360safe/titlewidget.cpp \
    $$PWD/360safe/tool_button.cpp \

RESOURCES += \
    $$PWD/360safe/Resources/360safe.qrc \

DISTFILES += \
    $$PWD/360safe/Resources/360safe_en.qm \
    $$PWD/360safe/Resources/360safe_zh.qm \
    $$PWD/360safe/Resources/qt_zh_CN.qm \
    $$PWD/360safe/Resources/360safe.qss \
    $$PWD/360safe/Resources/360safe_en.ts \
    $$PWD/360safe/Resources/360safe_zh.ts
