#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T13:38:26
#
#-------------------------------------------------

QT       -= gui

CONFIG += c++14

TARGET = Polygon
TEMPLATE = lib

DEFINES += POLYGON_LIBRARY

SOURCES += polygonexpansion.cpp \
    field.cpp \
    polygonio.cpp \
    polygonset.cpp

HEADERS += polygonexpansion.h \
        polygon_global.h \
    field.h \
    polygonio.h \
    polygonset.h

LIBS += -lboost_system -lboost_thread

INSTALLS += target

unix {
    target.path = /usr/lib
    target.extra = ldconfig
}
