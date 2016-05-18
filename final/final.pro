#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T12:07:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = final
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../AudioCapture.cpp \
    ../AudioInterface.cpp \
    ../AudioReceiver.cpp \
    ../Client.cpp \
    ../Server.cpp \
    ../VideoCamera.cpp \
    ../VideoDisplay.cpp \
    ../VideoMock.cpp \
    ../VideoReceive.cpp

HEADERS  += mainwindow.h \
    ../AudioCapture.h \
    ../AudioInterface.h \
    ../AudioReceiver.h \
    ../Video.h \
    ../videocamera.h \
    ../videodisplay.h

FORMS    += mainwindow.ui
