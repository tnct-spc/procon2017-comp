#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T17:48:35
#
#-------------------------------------------------

QT       += widgets

CONFIG += c++14

TARGET = DisplayAnswer
TEMPLATE = lib

CONFIG   += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = $$PWD/../Utilities/precompile.h

DEFINES += DISPLAYANSWER_LIBRARY

SOURCES += \
    answerboard.cpp \
    answerdock.cpp

HEADERS +=\
    answerboard.h \
    answerdock.h

LIBS += -L/usr/local/lib `pkg-config --libs opencv`

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    answerboard.ui \
    answerdock.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Polygon/release/ -lPolygon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Polygon/debug/ -lPolygon
else:unix: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon

INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Utilities/release/ -lUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Utilities/debug/ -lUtilities
else:unix: LIBS += -L$$OUT_PWD/../Utilities/ -lUtilities

INCLUDEPATH += $$PWD/../Utilities
DEPENDPATH += $$PWD/../Utilities
