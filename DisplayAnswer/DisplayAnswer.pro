QT += core
QT -= gui

CONFIG += c++11

TARGET = DisplayAnswer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    displayanswer.cpp

HEADERS += \
    displayanswer.h
