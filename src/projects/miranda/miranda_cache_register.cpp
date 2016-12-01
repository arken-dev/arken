#include <luajit-2.0/lua.hpp>
#include "mirandastate.h"

static int
miranda_cache_insert(lua_State *L) {
  const char * key   = luaL_checkstring(L, 1);
  const char * value = luaL_checkstring(L, 2);
  int expires = -1;
  if(lua_gettop(L) == 3) { /* número de argumentos */
    expires = lua_tointeger(L, 3);
  }
  MirandaState::insert(key, value, expires);
  return 0;
}

static int
miranda_cache_value(lua_State *L) {
  const char * key   = luaL_checkstring(L, 1);
  const char * value = MirandaState::value(key);
  if( value == 0 ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, value);
  }
  return 1;
}

static int
miranda_cache_remove(lua_State *L) {
  const char * key = luaL_checkstring(L, 1);
  lua_pushinteger(L, MirandaState::remove(key));
  return 0;
}

void
miranda_cache_register(lua_State * L) {
  static const luaL_reg Map[] = {
    {"insert", miranda_cache_insert},
    {"remove",  miranda_cache_remove},
    {"value",  miranda_cache_value},
    {NULL, NULL}
  };

  luaL_register(L, "cache", Map);
}
