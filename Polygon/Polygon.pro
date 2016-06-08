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

SOURCES += polygon.cpp \
    field.cpp

HEADERS += polygon.h\
        polygon_global.h \
    field.h

LIBS += -lboost_system -lboost_thread

unix {
    target.path = /usr/lib
    INSTALLS += target
}
