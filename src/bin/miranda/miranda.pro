QT         += core
QT         += network
CONFIG     += console
TEMPLATE    = app

TARGET      = miranda
DESTDIR     = ../../../bin

INCLUDEPATH += .
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../vendors/include


QMAKE_RPATHDIR += ../vendors
QMAKE_RPATHDIR += ../shared

win32:LIBS += ../../../vendors/lua51.dll
unix:LIBS += /usr/lib/x86_64-linux-gnu/libluajit-5.1.so
win32:LIBS += ../../../shared/oberon.dll
unix:LIBS += ../../../shared/liboberon.so

# Input
SOURCES += main.cpp   \
    mirandaserver.cpp \
    mirandatask.cpp   \
    mirandastate.cpp  \
    miranda_cache_register.cpp  \
    miranda_server_register.cpp \
    miranda_request_register.cpp

HEADERS += mirandaserver.h \
    mirandatask.h          \
    mirandastate.h
