#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T17:45:38
#
#-------------------------------------------------

QT       += widgets

CONFIG += c++14

TARGET = ImageRecognition
TEMPLATE = lib

CONFIG   += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = $$PWD/../Utilities/precompile.h

DEFINES += IMAGERECOGNITION_LIBRARY

SOURCES += imagerecognition.cpp

HEADERS += imagerecognition.h\
        imagerecognition_global.h

LIBS += -L/usr/local/lib `pkg-config --libs opencv`

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Polygon/release/ -lPolygon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Polygon/debug/ -lPolygon
else:unix: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon

INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon
