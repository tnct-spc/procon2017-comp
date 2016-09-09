#-------------------------------------------------
#
# Project created by QtCreator 2016-09-06T15:34:05
#
#-------------------------------------------------

QT       -= gui:

CONFIG += c++14

CONFIG += c++14

CONFIG += c++14

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
