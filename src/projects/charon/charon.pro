######################################################################
# Automatically generated by qmake (3.0) seg jan 25 10:12:08 2016
######################################################################

QT += core
QT -= gui

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = charon
DESTDIR = ../../../bin

INCLUDEPATH += .
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../deps/include


QMAKE_RPATHDIR += ../deps

LIBS += -L ../../../deps -llua -lcharon -lcurl

# Input
SOURCES += main.cpp charon.cpp

mac:QMAKE_POST_LINK += install_name_tool -change libcharon.1.dylib  @executable_path/../deps/libcharon.1.dylib ../../../bin/charon ; install_name_tool -change liblua.so  @executable_path/../deps/liblua.so ../../../bin/charon