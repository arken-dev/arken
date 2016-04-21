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


void
miranda_server_register(lua_State * L) {
  static const         luaL_reg Map[] = {
    {"version", miranda_server_version},
    {"reload",  miranda_server_reload},
    {"clear",   miranda_server_clear},
    {NULL, NULL}
  };

  luaL_register(L, "server", Map);
}
