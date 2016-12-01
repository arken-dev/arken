QT         += core
QT         += network
CONFIG     += console
CONFIG -= app_bundle

TEMPLATE    = app

TARGET      = miranda
DESTDIR     = ../../../bin

INCLUDEPATH += .
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../vendors/include
INCLUDEPATH += ../../../vendors/include/luajit-2.0
INCLUDEPATH += ../../lua/json


QMAKE_RPATHDIR += ../vendors

LIBS += -L ../../../vendors -llua -loberon -lcurl

# Input
SOURCES += main.cpp    \
    mirandacache.cpp   \
    mirandaserver.cpp  \
    mirandaservice.cpp \
    mirandastate.cpp   \
    mirandatask.cpp    \
    miranda_cache_register.cpp     \
    miranda_json_encode.c          \
    miranda_server_register.cpp    \
    miranda_service_register.cpp   \
    miranda_request_register.cpp   \
    miranda_task_register.cpp      \
    ../../lua/json/json_lock.cpp   \
    ../../lua/json/json.c   \
    ../../lua/json/strbuf.c \
    ../../lua/json/g_fmt.c  \
    ../../lua/json/dtoa.c

HEADERS += mirandaserver.h \
    mirandatask.h          \
    mirandastate.h         \
    ../../lua/json/json.h

mac:QMAKE_POST_LINK += install_name_tool -change liboberon.1.dylib  @executable_path/../vendors/liboberon.1.dylib ../../../bin/miranda ; install_name_tool -change liblua.so  @executable_path/../vendors/liblua.so ../../../bin/miranda
