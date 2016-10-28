#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T13:40:42
#
#-------------------------------------------------

QT += core
QT += gui
QT += widgets

TEMPLATE = app

TARGET = puck
DESTDIR = ../../../bin



SOURCES += main.cpp\
        dialog.cpp

unix:SOURCES += keylogger/linux.c
win32:SOURCES += keylogger/win32.c

SOURCES += keyloggerworker.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

unix:LIBS += -lX11

RESOURCES += \
    rescource.qrc
