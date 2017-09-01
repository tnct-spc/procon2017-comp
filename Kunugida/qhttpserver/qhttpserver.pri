INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += network

# Enable very detailed debug messages when compiling the debug version
CONFIG(debug, debug|release) {
    DEFINES += SUPERVERBOSE
}

HEADERS +=  $$PWD/src/qhttprequest.h \
            $$PWD/src/qhttpserverfwd.h \
            $$PWD/src/qhttpconnection.h \
            $$PWD/src/qhttpserver.h \
            $$PWD/src/qhttpresponse.h \
            $$PWD/src/qhttpserverapi.h \
            $$PWD/http-parser/http_parser.h

SOURCES +=  $$PWD/src/qhttpconnection.cpp \
            $$PWD/src/qhttpserver.cpp \
            $$PWD/src/qhttpresponse.cpp \
            $$PWD/src/qhttprequest.cpp \
            $$PWD/http-parser/http_parser.c
