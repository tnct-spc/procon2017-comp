QT += core
QT -= gui

CONFIG += c++14

TARGET = Polygon
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    polygonexpansion.cpp\
    field.cpp


HEADERS += \
    polygonexpansion.h\
    field.h


    target.path = /usr/local/lib
    target.extra = ldconfig
LIBS += -lboost_system -lboost_thread

unix {
    INSTALLS += target
}
