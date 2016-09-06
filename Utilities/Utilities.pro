#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T20:14:08
#
#-------------------------------------------------

QT       -= gui

TARGET = Utilities
TEMPLATE = lib

DEFINES += UTILITIES_LIBRARY

SOURCES += utilities.cpp

HEADERS += utilities.h\
        utilities_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
