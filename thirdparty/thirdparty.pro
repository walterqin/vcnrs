TEMPLATE = lib
TARGET = thirdparty

#include(../global.pri)

DEFINES += THIRDPARTY_LIB

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

HEADERS += \
    thirdparty_global.h \
    thirdparty.h \
    LBP/helper.hpp \
    LBP/lbp.hpp \
    mser/mser2.hpp \
    svm/precomp.hpp \
    xmlParser/xmlParser.h

SOURCES += \
    thirdparty.cpp \
    LBP/helper.cpp \
    LBP/lbp.cpp \
    mser/mser2.cpp \
    svm/corrected_svm.cpp \
    xmlParser/xmlParser.cpp

