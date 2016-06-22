#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T17:45:38
#
#-------------------------------------------------

QT       -= gui

CONFIG += c++14

TARGET = ImageRecognition
TEMPLATE = lib

DEFINES += IMAGERECOGNITION_LIBRARY

SOURCES += imagerecognition.cpp

HEADERS += imagerecognition.h\
        imagerecognition_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
