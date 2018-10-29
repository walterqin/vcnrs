#-------------------------------------------------
#
# Project created by QtCreator 2018-02-08T11:54:23
#
#-------------------------------------------------

QT       += core sql script widgets

TARGET = nrm
TEMPLATE = lib

INCLUDEPATH += \
    ../core \
    ../util \
    ../opencv/vc14/include \
    ../thirdparty/lbp \
    ../thirdparty/mser \
    ../thirdparty/svm \
    ../thirdparty/xmlParser

DEFINES += NRM_LIBRARY \

SOURCES += nrm.cpp \
    zonejudge.cpp \
    charsidentify.cpp \
    zonelocate.cpp \
    cparams.cpp \
    zonedetect.cpp \
    charssegment.cpp \
    charsrecognise.cpp \
    nrmworkerthread.cpp \
    nrmplugin.cpp \
    nrmparampage.cpp \
    config.cpp \
    corefunc.cpp \
    feature.cpp \
    trainann.cpp \
    trainannch.cpp \
    trainbase.cpp \
    trainsvm.cpp \
    trainmodelpage.cpp

HEADERS += nrm.h\
        nrm_global.h \
    zonejudge.h \
    charsidentify.h \
    zonelocate.h \
    cparams.h \
    zonedetect.h \
    charssegment.h \
    charsrecognise.h \
    nrmworkerthread.h \
    nrmplugin.h \
    nrmparampage.h \
    config.h \
    corefunc.h \
    feature.h \
    trainann.h \
    trainannch.h \
    trainbase.h \
    trainsvm.h \
    trainmodelpage.h

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

LIBS += -lcore -lutil -lthirdparty

FORMS += \
    nrmparampage.ui \
    trainmodelpage.ui
