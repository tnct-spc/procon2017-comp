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

SOURCES += \
    field.cpp \
    polygonio.cpp \
    expandedpolygon.cpp

HEADERS += \
        polygon_global.h \
    field.h \
    polygonio.h \
    expandedpolygon.h

LIBS += -lboost_system -lboost_thread

INSTALLS += target

unix {
    target.path = /usr/lib
    target.extra = ldconfig
}
