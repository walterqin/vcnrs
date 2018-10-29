TEMPLATE = app
TARGET = vcnts

QT += core widgets

INCLUDEPATH += ../core

CONFIG(debug, debug|release){
    DESTDIR =$$PWD/../debug
    LIBS += -L$$PWD/../debug
}else{
    DESTDIR =$$PWD/../release
    LIBS += -L$$PWD/../release
}

LIBS += -lAdvapi32

LIBS += -lcore

SOURCES += \
    main.cpp

RC_FILE += \
    app.rc
