TEMPLATE = lib
TARGET = core

QT       += sql script core gui widgets qml quick concurrent

DEFINES += CORE_LIB

RESOURCES += \
    core.qrc

#include(../global.pri)

INCLUDEPATH += ../opencv/vc14/include \
    ../nrm \
    ../thirdparty/lbp \
    ../thirdparty/mser \
    ../thirdparty/svm \
    ../thirdparty/xmlParser \
    ../util

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
    -lAdvapi32

RC_FILE = core.rc

HEADERS += \
    core.h \
    core_global.h \
    vcapp.h \
    vcactions.h \
    workbench.h \
    mainwindowbase.h \
    messagewidget.h \
    mainwindow.h \
    global.h \
    profile.h \
    serial.h \
    mainpagewidget.h \
    mainpageframe.h \
    optionframe.h \
    optionwidget.h \
    customdefinepagewidget.h \
    systemsetuppagewidget.h \
    systemoperationwidget.h \
    networkconfigwidget.h \
    generalparamwidget.h \
    instrumentparamwidget.h \
    simupageframe.h \
    simuwidget.h \
    logdataframe.h \
    logdatawidget.h \
    calibrationframe.h \
    calibrationwidget.h \
    charsrecogniseinterface.h \
    zonedetectinterface.h \
    numberrecognition.h \
    numberzone.h \
    character.h \
    numberrecognizeplugin.h \
    industrialcamera.h \
    worklist.h \
    checkstatusthread.h \
    matlist.h \
    cameradata.h \
    analysisresult.h \
    instrumentfactory.h \
    instrument.h \
    instrumentplugin.h \
    numberrecognitionfactory.h \
    paramheader.h \
    profilepage.h \
    taskframe.h \
    taskwidget.h

SOURCES += \
    core.cpp \
    vcapp.cpp \
    vcactions.cpp \
    workbench.cpp \
    mainwindowbase.cpp \
    messagewidget.cpp \
    mainwindow.cpp \
    global.cpp \
    profile.cpp \
    serial.cpp \
    mainpagewidget.cpp \
    mainpageframe.cpp \
    optionframe.cpp \
    optionwidget.cpp \
    customdefinepagewidget.cpp \
    systemsetuppagewidget.cpp \
    systemoperationwidget.cpp \
    networkconfigwidget.cpp \
    generalparamwidget.cpp \
    instrumentparamwidget.cpp \
    simupageframe.cpp \
    simuwidget.cpp \
    logdataframe.cpp \
    logdatawidget.cpp \
    calibrationframe.cpp \
    calibrationwidget.cpp \
    charsrecogniseinterface.cpp \
    zonedetectinterface.cpp \
    numberrecognition.cpp \
    numberzone.cpp \
    character.cpp \
    numberrecognizeplugin.cpp \
    industrialcamera.cpp \
    worklist.cpp \
    checkstatusthread.cpp \
    matlist.cpp \
    cameradata.cpp \
    analysisresult.cpp \
    instrumentfactory.cpp \
    instrument.cpp \
    instrumentplugin.cpp \
    numberrecognitionfactory.cpp \
    profilepage.cpp \
    taskframe.cpp \
    taskwidget.cpp

FORMS += \
    mainwindow.ui \
    messagewidget.ui \
    mainpagewidget.ui \
    networkconfigwidget.ui \
    systemsetuppagewidget.ui \
    generalparamwidget.ui \
    systemoperationwidget.ui \
    instrumentparamwidget.ui \
    simuwidget.ui \
    calibrationwidget.ui \
    profilepage.ui

