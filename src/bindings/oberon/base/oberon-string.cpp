#include <luajit-2.0/lua.hpp>
#include <oberon/base>
#include <stdlib.h>

static int lua_oberon_string_ends_with( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  int result         = string::endsWith(string, str);
  lua_pushboolean(L, result);
  return 1;
}

static int lua_oberon_string_insert( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int len            = luaL_checkinteger(L, 2);
  const char *ba     = luaL_checkstring(L, 3);
  char *result = string::insert(string, len, ba);
  lua_pushstring(L, result);
  free(result);
  return 1;
}

int luaopen_oberon_string( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"endsWith", lua_oberon_string_ends_with},
    {"insert",   lua_oberon_string_insert},
    {NULL, NULL}
  };
  luaL_register(L, "string", Map);
  return 1;
}
