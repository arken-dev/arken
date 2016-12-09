#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T13:40:42
#
#-------------------------------------------------

QT += core
QT += gui
QT += widgets

CONFIG -= app_bundle

TEMPLATE = app

TARGET = callisto
DESTDIR = ../../../bin

INCLUDEPATH += .
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../deps/include

SOURCES += main.cpp\
        dialog.cpp \
        callisto.cpp

# unix:SOURCES += keylogger/linux.c
# win32:SOURCES += keylogger/win32.c
# SOURCES += keyloggerworker.cpp

SOURCES += ../charon/charon.cpp

HEADERS  += dialog.h callisto.h

FORMS    += dialog.ui

#unix:LIBS += -lX11

RESOURCES += \
    rescource.qrc

QMAKE_RPATHDIR += ../deps

LIBS += -L ../../../deps -lcharon -llua -lcurl

mac:QMAKE_POST_LINK += install_name_tool -change libcharon.1.dylib  @executable_path/../deps/libcharon.1.dylib ../../../bin/callisto ; install_name_tool -change liblua.so  @executable_path/../deps/liblua.so ../../../bin/callisto
