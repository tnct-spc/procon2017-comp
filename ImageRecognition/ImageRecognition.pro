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

SOURCES += imagerecognition.cpp \
    imagerecongnitionwithhumanpower.cpp \
    mygraphicsview.cpp

HEADERS += imagerecognition.h\
        imagerecognition_global.h \
    imagerecongnitionwithhumanpower.h \
    mygraphicsview.h

LIBS += -L/usr/lib `pkg-config --libs opencv`

unix {
    target.path = /usr/lib
    INSTALLS += target
}

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

INCLUDEPATH += $$PWD/../spdlog/include
DEPENDPATH += $$PWD/../spdlog/include

FORMS += \
    imagerecongnitionwithhumanpower.ui \
    mygraphicsview.ui
