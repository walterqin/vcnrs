#-------------------------------------------------
#
# Project created by QtCreator 2018-05-09T14:44:30
#
#-------------------------------------------------

QT       += core widgets sql

TARGET = dalsag3gc11
TEMPLATE = lib

DEFINES += CAMERA_LIBRARY

INCLUDEPATH += ../core \
    ../opencv/vc14/include

#CONFIG += plugin

CONFIG(debug, debug|release){
    DESTDIR =$$PWD/../debug
    LIBS += -L$$PWD/../debug
}else{
    DESTDIR =$$PWD/../release
    LIBS += -L$$PWD/../release
}

LIBS += -lcore
LIBS += -L"../opencv/vc14/lib" \
    -lopencv_world340 \
    -lopencv_world340d \
    -lAdvapi32

SOURCES += camera.cpp \
    dalsag3gc11.cpp \
    workerthreaddalsag3gc11.cpp \
    parampage.cpp \
    debugpage.cpp \
    connectionpage.cpp \
    dalsag3gc11plugin.cpp

HEADERS += camera.h\
        camera_global.h \
    dalsag3gc11.h \
    workerthreaddalsag3gc11.h \
    parampage.h \
    debugpage.h \
    ringbuffer.h \
    connectionpage.h \
    dalsag3gc11plugin.h

FORMS += \
    debugpage.ui \
    parampage.ui \
    connectionpage.ui
