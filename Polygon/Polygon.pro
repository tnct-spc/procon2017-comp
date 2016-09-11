#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T13:38:26
#
#-------------------------------------------------

QT       += widgets

CONFIG += c++14

TARGET = Polygon
TEMPLATE = lib

DEFINES += POLYGON_LIBRARY

SOURCES += \
    field.cpp \
    polygonio.cpp \
    expandedpolygon.cpp \
    singlepolygondisplay.cpp \
    polygonviewer.cpp

HEADERS += \
        polygon_global.h \
    field.h \
    polygonio.h \
    expandedpolygon.h \
    singlepolygondisplay.h \
    polygonviewer.h

LIBS += -lboost_system -lboost_thread

INSTALLS += target

unix {
    target.path = /usr/lib
    target.extra = ldconfig
}

FORMS += \
    singlepolygondisplay.ui \
    form.ui
