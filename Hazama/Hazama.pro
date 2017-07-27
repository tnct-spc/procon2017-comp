#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T00:36:30
#
#-------------------------------------------------

QT       += core gui widgets network

CONFIG += c++14

TARGET = Hazama
TEMPLATE = app

CONFIG   += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = $$PWD/../Utilities/precompile.h

SOURCES += main.cpp\
        hazama.cpp \
    http/answer_form.cpp \
    http/help_page.cpp \
    http/output_problem_page.cpp \
    http/request_mapper.cpp

HEADERS  += hazama.h \
    http/answer_form.h \
    http/help_page.h \
    http/output_problem_page.h \
    http/request_mapper.h

FORMS    += hazama.ui

LIBS += -L/usr/local/lib `pkg-config --libs opencv`

unix:!macx: LIBS += -L$$OUT_PWD/../Polygon/ -lPolygon
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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Solver/release/ -lSolver
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Solver/debug/ -lSolver
else:unix: LIBS += -L$$OUT_PWD/../Solver/ -lSolver

INCLUDEPATH += $$PWD/../Solver
DEPENDPATH += $$PWD/../Solver


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Utilities/release/ -lUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Utilities/debug/ -lUtilities
else:unix: LIBS += -L$$OUT_PWD/../Utilities/ -lUtilities

INCLUDEPATH += $$PWD/../Utilities
DEPENDPATH += $$PWD/../Utilities

include(qhttpserver/qhttpserver.pri)

unix {
    target.path = /usr/local/bin
    INSTALLS += target
}
