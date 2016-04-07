QT         += core
QT         += network
CONFIG     += console
TEMPLATE    = app

TARGET      = miranda
DESTDIR     = ../../../bin

INCLUDEPATH += .
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../vendors/include


QMAKE_RPATHDIR=../../../vendors

win32:LIBS += ../../../vendors/lua51.dll
unix:LIBS += /usr/lib/x86_64-linux-gnu/libluajit-5.1.so

# Input
SOURCES += main.cpp \
    mirandaserver.cpp \
    mirandatask.cpp \
    mirandastate.cpp

HEADERS += \
    mirandaserver.h \
    mirandatask.h \
    mirandastate.h
