#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T13:38:26
#
#-------------------------------------------------

QT       += widgets

CONFIG += c++14

TARGET = Polygon
TEMPLATE = lib

CONFIG   += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = $$PWD/../Utilities/precompile.h

DEFINES += POLYGON_LIBRARY

SOURCES += \
    field.cpp \
    polygonio.cpp \
    expandedpolygon.cpp \
    singlepolygondisplay.cpp \
    polygonviewer.cpp \
    fit.cpp \
    neoexpandedpolygon.cpp \
    neosinglepolygondisplay.cpp \
    neopolygonviewer.cpp \
    form.cpp \
    neofield.cpp \
    polygon.cpp \
    complexangle.cpp \
    neopolygonio.cpp \
    Evaluation/evaluate.cpp \
    Evaluation/evaluation.cpp \
    Utils/polygonconnector.cpp \
    Utils/checknearly.cpp

HEADERS += \
        polygon_global.h \
    field.h \
    polygonio.h \
    expandedpolygon.h \
    singlepolygondisplay.h \
    polygonviewer.h \
    fit.h \
    neoexpandedpolygon.h \
    neosinglepolygondisplay.h \
    neopolygonviewer.h \
    form.h \
    neofield.h \
    polygon.h \
    complexangle.h \
    neopolygonio.h \
    Evaluation/evaluate.h \
    Evaluation/evaluation.h \
    Utils/polygonconnector.h \
    Utils/checknearly.h

LIBS += -lboost_system -lboost_thread
LIBS += -L/usr/lib `pkg-config --libs opencv`

INSTALLS += target

unix {
    target.path = /usr/lib
    target.extra = ldconfig
}

FORMS += \
    singlepolygondisplay.ui \
    form.ui \
    neosinglepolygondisplay.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Utilities/release/ -lUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Utilities/debug/ -lUtilities
else:unix: LIBS += -L$$OUT_PWD/../Utilities/ -lUtilities

INCLUDEPATH += $$PWD/../Utilities
DEPENDPATH += $$PWD/../Utilities

INCLUDEPATH += $$PWD/../spdlog/include
DEPENDPATH += $$PWD/../spdlog/include

INCLUDEPATH += $$PWD/../NeoSolver
DEPENDPATH += $$PWD/../NeoSolver
