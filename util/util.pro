#-------------------------------------------------
#
# Project created by QtCreator 2018-02-13T10:53:15
#
#-------------------------------------------------

QT       += core

TARGET = util
TEMPLATE = lib

DEFINES += UTIL_LIBRARY

SOURCES += util.cpp \
    kv.cpp \
    utilities.cpp

HEADERS += util.h\
        util_global.h \
    kv.h \
    utilities.h

INCLUDEPATH += ../opencv/vc14/include

CONFIG(debug, debug|release){
    DESTDIR =$$PWD/../debug
    LIBS += -L$$PWD/../debug
}else{
    DESTDIR =$$PWD/../release
    LIBS += -L$$PWD/../release
}

LIBS += -L"../opencv/vc14/lib" \
    -lopencv_world340 \
    -lopencv_world340d \
