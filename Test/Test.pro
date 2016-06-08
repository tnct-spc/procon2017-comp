#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T13:46:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    testpolygon.cpp

HEADERS  += mainwindow.h \
    testpolygon.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon
INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon
