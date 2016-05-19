#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T12:07:25
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = final
TEMPLATE = app


SOURCES +=./AudioCapture.cpp \
    ./AudioInterface.cpp \
    ./AudioReceiver.cpp \
    ./VideoCamera.cpp \
    ./VideoDisplay.cpp \
    ./VideoMock.cpp \
    ./VideoReceive.cpp

HEADERS  +=./AudioCapture.h \
    ./AudioInterface.h \
    ./AudioReceiver.h \
    ./videocamera.h \
    ./videodisplay.h
