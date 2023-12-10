#-------------------------------------------------
#
# Project created by QtCreator 2010-11-09T22:45:19
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = netttts
TEMPLATE = app


SOURCES += main.cpp\
    game.cpp \
        mainwindow.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    game.h \
    server.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe40619ad
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
