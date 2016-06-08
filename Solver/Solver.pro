#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T17:49:06
#
#-------------------------------------------------

QT       -= gui

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
