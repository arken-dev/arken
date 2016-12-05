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
INCLUDEPATH += ../../../vendors/include

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

QMAKE_RPATHDIR += ../vendors

LIBS += -L ../../../vendors -lcharon -llua -lcurl

mac:QMAKE_POST_LINK += install_name_tool -change libcharon.1.dylib  @executable_path/../vendors/libcharon.1.dylib ../../../bin/callisto ; install_name_tool -change liblua.so  @executable_path/../vendors/liblua.so ../../../bin/callisto
