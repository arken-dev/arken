#include <luajit-2.0/lua.hpp>
#include <oberon/helper>
#include <oberon/modules/regex.h>

static int lua_oberon_regex_match( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * regex  = luaL_checkstring(L, 2);
  bool result = regex::match(string, regex);
  lua_pushboolean( L, result );
  return 1;
}

static int lua_oberon_regex_replace( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * regex  = luaL_checkstring(L, 2);
  const char * after  = luaL_checkstring(L, 3);
  char * result = regex::replace(string, regex, after);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

extern "C" {
  int luaopen_oberon_regex( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"match",   lua_oberon_regex_match},
      {"replace", lua_oberon_regex_replace},
      {NULL, NULL}
    };
    luaL_register(L, "regex", Map);
    return 1;
  }
}
