#include <luajit-2.0/lua.hpp>
#include <QDebug>
#include "mirandastate.h"

static int
miranda_cache_insert(lua_State *L) {
  const char * key   = luaL_checkstring(L, 1);
  const char * value = luaL_checkstring(L, 2);
  MirandaState::insert(key, value);
  return 0;
}

static int
miranda_cache_value(lua_State *L) {
  const char * key = luaL_checkstring(L, 1);
  const char * value = MirandaState::value(key);
  lua_pushstring(L, value);
  return 1;
}

void
miranda_cache_register(lua_State * L) {
  static const         luaL_reg Map[] = {
    {"insert", miranda_cache_insert},
    {"value",  miranda_cache_value},
    {NULL, NULL}
  };

  luaL_register(L, "cache", Map);
}
