#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T12:07:25
#
#-------------------------------------------------

QT       += core gui

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
	
# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/imageviewer
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS Video.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/imageviewer
INSTALLS += target sources

symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)

#Symbian has built-in component named imageviewer so we use different target
symbian: TARGET = imageviewerexample

wince*: {
   DEPLOYMENT_PLUGIN += qjpeg qmng qgif
}
maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)

symbian: warning(This example might not fully work on Symbian platform)
maemo5: warning(This example might not fully work on Maemo platform)
simulator: warning(This example might not fully work on Simulator platform)

unix: CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += -std=c++11
unix: PKGCONFIG += opencv