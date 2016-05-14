QT += core
QT -= gui

CONFIG += c++11

TARGET = Solver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    algorithmwrapper.cpp \
    Algorithm/simplealgorithm.cpp

HEADERS += \
    algorithmwrapper.h \
    Algorithm/simplealgorithm.h
