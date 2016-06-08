QT += core
QT -= gui

CONFIG += c++14

TARGET = Polygon
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    field.cpp \
    polygonexpansion.cpp

HEADERS += \
    field.h \
    polygonexpansion.h

LIBS += -lboost_system -lboost_thread

unix {
    target.path = /usr/local/lib
    target.extra = ldconfig
    INSTALLS += target
}
