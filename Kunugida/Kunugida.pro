#-------------------------------------------------
#
# Project created by QtCreator 2017-04-14T16:03:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kunugida
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG   += precompile_header
CONFIG += c++14


SOURCES += main.cpp\
        kunugida.cpp

HEADERS  += kunugida.h

FORMS    += kunugida.ui

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = $$PWD/../Utilities/precompile.h

LIBS += -L/usr/lib `pkg-config --libs opencv`

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Polygon/release/ -lPolygon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Polygon/debug/ -lPolygon
else:unix: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon

INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ImageRecognition/release/ -lImageRecognition
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ImageRecognition/debug/ -lImageRecognition
else:unix: LIBS += -L$$OUT_PWD/../ImageRecognition/ -lImageRecognition

INCLUDEPATH += $$PWD/../ImageRecognition
DEPENDPATH += $$PWD/../ImageRecognition

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DisplayAnswer/release/ -lDisplayAnswer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DisplayAnswer/debug/ -lDisplayAnswer
else:unix: LIBS += -L$$OUT_PWD/../DisplayAnswer/ -lDisplayAnswer

INCLUDEPATH += $$PWD/../DisplayAnswer
DEPENDPATH += $$PWD/../DisplayAnswer

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Utilities/release/ -lUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Utilities/debug/ -lUtilities
else:unix: LIBS += -L$$OUT_PWD/../Utilities/ -lUtilities

INCLUDEPATH += $$PWD/../Utilities
DEPENDPATH += $$PWD/../Utilities

INCLUDEPATH += $$PWD/../spdlog/include
DEPENDPATH += $$PWD/../spdlog/include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QRcode/release/ -lQRcode
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QRcode/debug/ -lQRcode
else:unix: LIBS += -L$$OUT_PWD/../QRcode/ -lQRcode

INCLUDEPATH += $$PWD/../QRcode
DEPENDPATH += $$PWD/../QRCode

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ProbMaker/release/ -lProbMaker
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ProbMaker/debug/ -lProbMaker
else:unix: LIBS += -L$$OUT_PWD/../ProbMaker/ -lProbMaker

INCLUDEPATH += $$PWD/../ProbMaker
DEPENDPATH += $$PWD/../ProbMaker

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NeoSolver/release/ -lNeoSolver
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NeoSolver/debug/ -lNeoSolver
else:unix: LIBS += -L$$OUT_PWD/../NeoSolver/ -lNeoSolver

include(qhttpserver/qhttpserver.pri)

INCLUDEPATH += $$PWD/../NeoSolver
DEPENDPATH += $$PWD/../NeoSolver
