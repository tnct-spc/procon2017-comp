#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T03:12:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageRecognition
TEMPLATE = app


SOURCES += main.cpp\
        imagerecognition.cpp

HEADERS  += imagerecognition.h

FORMS    += imagerecognition.ui

LIBS += -L/usr/local/lib `pkg-config --libs opencv`
