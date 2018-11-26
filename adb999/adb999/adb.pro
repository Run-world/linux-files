#-------------------------------------------------
#
# Project created by QtCreator 2018-08-03T10:52:36
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE = res.rc
TARGET = Debug1
TEMPLATE = app

LIBS += -lws2_32
SOURCES += main.cpp\
        mainwindow.cpp \
    adb_find.cpp \
    mythread.cpp \
    logthread.cpp \
    remotecontrl.cpp \
    sendthread.cpp \
    listenthread.cpp \
    kmsgthread.cpp \
    shell.cpp

HEADERS  += mainwindow.h \
    adb_find.h \
    mythread.h \
    logthread.h \
    remotecontrl.h \
    sendthread.h \
    listenthread.h \
    kmsgthread.h \
    shell.h

FORMS    += mainwindow.ui \
    remotecontrl.ui

RESOURCES += \
    image.qrc
