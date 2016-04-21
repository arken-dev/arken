#include <luajit-2.0/lua.hpp>
#include <QDebug>
#include <QThread>
#include "mirandastate.h"

static int
miranda_server_reload(lua_State *) {
  MirandaState::reload();
  qDebug() << "reload: " << MirandaState::version() ;
  return 0;
}

static int
miranda_server_clear(lua_State *) {
  MirandaState::clear();
  qDebug() << "clear: " << MirandaState::version() ;
  return 0;
}

static int
miranda_server_version(lua_State *L) {
  lua_pushnumber(L, MirandaState::version());
  return 1;
}

static int
miranda_server_task(lua_State *L) {
  const char * file_name = luaL_checkstring(L, 1);
  const char * uuid = luaL_checkstring(L, 2);
  MirandaState::createTask( file_name, uuid );
  return 0;
}

static int
miranda_server_service(lua_State *L) {
  const char * file_name = luaL_checkstring(L, 1);
  MirandaState::createService( file_name );
  return 0;
}

void
miranda_server_register(lua_State * L) {
  static const         luaL_reg Map[] = {
    {"version", miranda_server_version},
    {"reload",  miranda_server_reload},
    {"clear",   miranda_server_clear},
    {"task",    miranda_server_task},
    {"service", miranda_server_service},
    {NULL, NULL}
  };

  luaL_register(L, "server", Map);
}
