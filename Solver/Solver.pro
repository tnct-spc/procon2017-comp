#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T17:49:06
#
#-------------------------------------------------

QT       -= gui

CONFIG += c++14

TARGET = Solver
TEMPLATE = lib

DEFINES += SOLVER_LIBRARY

SOURCES += solver.cpp \
    Algorithm/algorithmwrapper.cpp \
    Algorithm/simplealgorithm.cpp

HEADERS += solver.h\
        solver_global.h \
    Algorithm/algorithmwrapper.h \
    Algorithm/simplealgorithm.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Polygon/release/ -lPolygon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Polygon/debug/ -lPolygon
else:unix: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon

INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon
