#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T21:35:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paintings
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    timer.cpp \
    cadran.cpp \
    thermometer.cpp \
    i2cbusses.cpp \
    getsensors.cpp

HEADERS  += mainwindow.h \
    timer.h \
    cadran.h \
    thermometer.h \
    i2c-dev.h \
    i2cbusses.h \
    getsensors.h

FORMS    += mainwindow.ui

DISTFILES += \
    C.png

