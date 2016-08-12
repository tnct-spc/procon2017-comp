#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T17:48:35
#
#-------------------------------------------------

QT       += widgets

CONFIG += c++14

TARGET = DisplayAnswer
TEMPLATE = lib

DEFINES += DISPLAYANSWER_LIBRARY

SOURCES += \
    answerboard.cpp

HEADERS +=\
        displayanswer_global.h \
    answerboard.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    answerboard.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Polygon/release/ -lPolygon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Polygon/debug/ -lPolygon
else:unix: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon

INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon
