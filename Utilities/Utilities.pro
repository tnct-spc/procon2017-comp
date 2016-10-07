#-------------------------------------------------
#
# Project created by QtCreator 2016-09-06T15:34:05
#
#-------------------------------------------------

QT       += core gui widgets network

CONFIG += c++14

TARGET = Utilities
TEMPLATE = lib

CONFIG   += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = $$PWD/../Utilities/precompile.h

DEFINES += UTILITIES_LIBRARY

SOURCES += utilities.cpp \
    parallel.cpp \
    threshold.cpp

HEADERS += utilities.h\
        utilities_global.h \
    precompile.h \
    parallel.h \
    threshold.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
