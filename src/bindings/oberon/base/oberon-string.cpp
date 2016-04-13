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

static int lua_oberon_string_repeated( lua_State *L ) {
  char * string = (char *) luaL_checkstring(L, 1);
  int    times  =  luaL_checkinteger(L, 2);
  char * result = string::repeated(string, times);
  lua_pushstring(L, result);  /* push result */
  free(result);
  return 1;
}

static int lua_oberon_string_right( lua_State *L ) {
  char * string = (char *) luaL_checkstring(L, 1);
  int    len    =  luaL_checkinteger(L, 2);
  char * result = string::right(string, len);
  lua_pushstring(L, result);  /* push result */
  free(result);
  return 1;
}

static int lua_oberon_string_simplified( lua_State *L ) {
  char * string = (char *) luaL_checkstring(L, 1);
  char * result = string::simplified(string);
  lua_pushstring(L, result);  /* push result */
  free(result);
  return 1;
}

static int lua_oberon_string_startsWith( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * ba     = luaL_checkstring(L, 2);
  int  result         = string::startsWith(string, ba);
  lua_pushboolean(L, result);  /* push result */
  return 1;
}

int luaopen_oberon_string( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"append",     lua_oberon_string_append},
    {"endsWith",   lua_oberon_string_endsWith},
    {"insert",     lua_oberon_string_insert},
    {"mid",        lua_oberon_string_mid},
    {"repeated",   lua_oberon_string_repeated},
    {"right",      lua_oberon_string_right},
    {"simplified", lua_oberon_string_simplified},
    {"startsWith", lua_oberon_string_startsWith},
    {NULL, NULL}
  };
  luaL_register(L, "string", Map);
  return 1;
}
