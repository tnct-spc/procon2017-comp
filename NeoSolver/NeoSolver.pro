#-------------------------------------------------
#
# Project created by QtCreator 2017-07-27T15:15:52
#
#-------------------------------------------------

QT       += gui widgets network

TARGET = NeoSolver
TEMPLATE = lib

DEFINES += NEOSOLVER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = $$PWD/../Utilities/precompile.h

CONFIG   += precompile_header
CONFIG += c++14

LIBS += -L/usr/lib `pkg-config --libs opencv`
LIBS += -lboost_system -lboost_thread

SOURCES += neosolver.cpp \
    Algorithm/algorithmwrapper.cpp \
    Algorithm/testalgortithm.cpp \
    Algorithm/beamsearch.cpp \
    Evaluation/evaluation.cpp \
    Utils/polygonconnector.cpp \
    Evaluation/evaluate.cpp

HEADERS += neosolver.h\
    neosolver_global.h \
    Algorithm/algorithmwrapper.h \
    Algorithm/testalgortithm.h \
    Algorithm/beamsearch.h \
    Evaluation/evaluation.h \
    Utils/polygonconnector.h \
    Evaluation/evaluate.h

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

INCLUDEPATH += $$PWD/../DisplayAnswer
DEPENDPATH += $$PWD/../DisplayAnswer

INCLUDEPATH += $$PWD/../spdlog/include
DEPENDPATH += $$PWD/../spdlog/include

unix {
    target.path = /usr/lib
    INSTALLS += target
}
