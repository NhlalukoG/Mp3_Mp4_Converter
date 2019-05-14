#-------------------------------------------------
#
# Project created by QtCreator 2018-09-25T00:44:52
#
#-------------------------------------------------

QT       += core gui
#QT       -= console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = Mp3ToMp4_Convertor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    converter.cpp \
    countfiles.cpp

HEADERS  += mainwindow.h \
    converter.h \
    countfiles.h

FORMS    += mainwindow.ui

DISTFILES += \
    README.md

RESOURCES += \
    resources.qrc
