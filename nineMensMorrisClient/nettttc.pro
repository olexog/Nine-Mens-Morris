#-------------------------------------------------
#
# Project created by QtCreator 2010-11-09T20:37:09
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = nettttc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tttwidget.cpp \
    nethandler.cpp

HEADERS  += mainwindow.h \
    tttwidget.h \
    nethandler.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe2a36659
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
