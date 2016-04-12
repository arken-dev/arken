#include <luajit-2.0/lua.hpp>
#include <oberon/base>
#include <stdlib.h>

static int lua_oberon_string_append( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  char *result = string::append(string, str);
  lua_pushstring(L, result);
  free(result);
  return 1;
}

static int lua_oberon_string_endsWith( lua_State *L ) {
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

static int lua_oberon_string_mid( lua_State *L ) {
  size_t string_len;
  char *string = (char *) luaL_checklstring(L, 1, &string_len);
  int pos      =  luaL_checkinteger(L, 2);
  int len;
  if(lua_gettop(L) == 3) { /* número de argumentos */
    len =  luaL_checkinteger(L, 3);
  } else {
    len = string_len;
  }
  char *result = string::mid(string, string_len, pos, len);
  lua_pushstring(L, result);  /* push result */
  free(result);
  return 1;
}


int luaopen_oberon_string( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"append",   lua_oberon_string_append},
    {"endsWith", lua_oberon_string_endsWith},
    {"insert",   lua_oberon_string_insert},
    {"mid",      lua_oberon_string_mid},
    {NULL, NULL}
  };
  luaL_register(L, "string", Map);
  return 1;
}
