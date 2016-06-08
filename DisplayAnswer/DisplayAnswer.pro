#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T17:48:35
#
#-------------------------------------------------

QT       += widgets

TARGET = DisplayAnswer
TEMPLATE = lib

DEFINES += DISPLAYANSWER_LIBRARY

SOURCES += displayanswer.cpp

HEADERS += displayanswer.h\
        displayanswer_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
