#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T17:49:06
#
#-------------------------------------------------

QT       += core gui widgets

CONFIG += c++14

TARGET = Solver
TEMPLATE = lib

DEFINES += SOLVER_LIBRARY

SOURCES += solver.cpp \
    Algorithm/algorithmwrapper.cpp \
    Algorithm/simplealgorithm.cpp \
    Algorithm/pooralgorithm.cpp \
    Algorithm/hillclibming.cpp \
    Utils/polygonconnector.cpp \
    Utils/fit.cpp

HEADERS += solver.h\
        solver_global.h \
    Algorithm/algorithmwrapper.h \
    Algorithm/simplealgorithm.h \
    Algorithm/pooralgorithm.h \
    Algorithm/hillclibming.h \
    Utils/polygonconnector.h \
    Utils/fit.h

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
