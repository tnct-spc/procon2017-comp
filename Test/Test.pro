#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T13:46:45
#
#-------------------------------------------------

QT       += core gui widgets

CONFIG += c++14

TARGET = Test
TEMPLATE = app

CONFIG   += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = $$PWD/../Utilities/precompile.h

LIBS += -L/usr/lib `pkg-config --libs opencv`

SOURCES += main.cpp\
    testdisplayanswer.cpp \
    testjoinpolygon.cpp \
    testerwraper.cpp \
    testsearchsamelength.cpp

HEADERS += testdisplayanswer.h\
    testjoinpolygon.h \
    testerwraper.h \
    testsearchsamelength.h

unix:!macx: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon
INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon


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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Solver/release/ -lSolver
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Solver/debug/ -lSolver
else:unix: LIBS += -L$$OUT_PWD/../Solver/ -lSolver

INCLUDEPATH += $$PWD/../Solver
DEPENDPATH += $$PWD/../Solver

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Polygon/release/ -lPolygon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Polygon/debug/ -lPolygon
else:unix: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ImageRecognition/release/ -lImageRecognition
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ImageRecognition/debug/ -lImageRecognition
else:unix: LIBS += -L$$OUT_PWD/../ImageRecognition/ -lImageRecognition

INCLUDEPATH += $$PWD/../ImageRecognition
DEPENDPATH += $$PWD/../ImageRecognition
