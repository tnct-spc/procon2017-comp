#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T00:49:53
#
#-------------------------------------------------

QT       += core gui widgets

CONFIG += c++14

TARGET = ProbMaker
TEMPLATE = app

CONFIG   += precompile_header


# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = $$PWD/../Utilities/precompile.h

SOURCES += main.cpp\
        probmaker.cpp

HEADERS  += probmaker.h

FORMS    += probmaker.ui

CONFIG   += precompile_header

LIBS += -L/usr/local/lib `pkg-config --libs opencv`

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Polygon/release/ -lPolygon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Polygon/debug/ -lPolygon
else:unix: LIBS += -L$$OUT_PWD/../Polygon -lPolygon

INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DisplayAnswer/release/ -lDisplayAnswer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DisplayAnswer/debug/ -lDisplayAnswer
else:unix: LIBS += -L$$OUT_PWD/../DisplayAnswer/ -lDisplayAnswer

INCLUDEPATH += $$PWD/../DisplayAnswer
DEPENDPATH += $$PWD/../DisplayAnswer


<<<<<<< HEAD
INCLUDEPATH += $$PWD/../spdlog/include
DEPENDPATH += $$PWD/../spdlog/include
=======
>>>>>>> 121d3d6a2fdffef63196c3dcd970a39586d0e685
