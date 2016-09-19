#-------------------------------------------------
#
# Project created by QtCreator 2016-09-04T17:19:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Slave
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Polygon/release/ -lPolygon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Polygon/debug/ -lPolygon
else:unix: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon

INCLUDEPATH += $$PWD/../Polygon
DEPENDPATH += $$PWD/../Polygon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Solver/release/ -lSolver
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Solver/debug/ -lSolver
else:unix: LIBS += -L$$OUT_PWD/../Solver/ -lSolver

INCLUDEPATH += $$PWD/../Solver
DEPENDPATH += $$PWD/../Solver
