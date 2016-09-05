#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T20:14:08
#
#-------------------------------------------------

QT       -= gui

TARGET = Utility
TEMPLATE = lib

DEFINES += UTILITY_LIBRARY

SOURCES += utility.cpp

HEADERS += utility.h\
        utility_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
