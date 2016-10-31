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

INCLUDEPATH += .
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../vendors/include

SOURCES += main.cpp\
        dialog.cpp \
        puck.cpp

unix:SOURCES += keylogger/linux.c
win32:SOURCES += keylogger/win32.c

SOURCES += keyloggerworker.cpp

HEADERS  += dialog.h puck.h

FORMS    += dialog.ui

unix:LIBS += -lX11

RESOURCES += \
    rescource.qrc

QMAKE_RPATHDIR += ../vendors

win32:LIBS += ../../../vendors/lua51.dll
win32:LIBS += ../../../vendors/oberon.dll
unix:LIBS += /usr/lib/x86_64-linux-gnu/libluajit-5.1.so
unix:LIBS += ../../../vendors/liboberon.so
