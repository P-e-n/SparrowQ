#-------------------------------------------------
#
# Project created by QtCreator 2018-03-30T14:20:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SparrowQ
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        sparrow.cpp \
        dothinkey.cpp \
        iniparser.cpp \
        logger.cpp

HEADERS += \
        sparrow.h \
        dothinkey.h \
        iniparser.h \
        logger.h

FORMS += \
        sparrow.ui

LIBS += -L$$PWD/../libs/DTCCM2_SDK/X64_Lib/ -ldtccm2

INCLUDEPATH += $$PWD/../libs/DTCCM2_SDK
DEPENDPATH += $$PWD/../libs/DTCCM2_SDK

RESOURCES += \
        assets.qrc

win32: RC_ICONS = sparrow.ico
