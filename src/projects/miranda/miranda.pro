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

win32:LIBS += ../../../vendors/lua51.dll
unix:LIBS += /usr/lib/x86_64-linux-gnu/libluajit-5.1.so
win32:LIBS += ../../../vendors/oberon.dll
unix:LIBS += ../../../vendors/liboberon.so

# Input
SOURCES += main.cpp    \
    mirandaserver.cpp  \
    mirandaservice.cpp \
    mirandastate.cpp   \
    mirandatask.cpp    \
    miranda_cache_register.cpp   \
    miranda_server_register.cpp  \
    miranda_service_register.cpp \
    miranda_request_register.cpp

HEADERS += mirandaserver.h \
    mirandatask.h          \
    mirandastate.h
