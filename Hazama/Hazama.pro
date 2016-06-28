#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T00:36:30
#
#-------------------------------------------------

QT       += core gui widgets

CONFIG += c++14

TARGET = Hazama
TEMPLATE = app


SOURCES += main.cpp\
        hazama.cpp

HEADERS  += hazama.h

FORMS    += hazama.ui

LIBS += -L/usr/local/lib `pkg-config --libs opencv`

unix:!macx: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon
INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon
