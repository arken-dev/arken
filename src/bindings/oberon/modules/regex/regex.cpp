// Copyright 2016 The Oberon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <oberon/helper>
#include <oberon/modules/regex.h>

static int lua_oberon_regex_match( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * regex  = luaL_checkstring(L, 2);
  bool result = regex::match(string, regex);
  lua_pushboolean( L, result );
  return 1;
}

static int lua_oberon_regex_ematch( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * regex  = luaL_checkstring(L, 2);
  bool result = regex::ematch(string, regex);
  lua_pushboolean( L, result );
  return 1;
}

static int lua_oberon_regex_index( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * regex  = luaL_checkstring(L, 2);
  int result;
  if(lua_gettop(L) == 3) {
    int offset = luaL_checkint(L, 3);
    result = regex::index(string, regex, offset);
  } else {
    result = regex::index(string, regex);
  }
  lua_pushinteger( L, result );
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

static int lua_oberon_regex_scan( lua_State *L ) {
  OStringList * list;
  const char  * string = luaL_checkstring(L, 1);
  const char  * regex  = luaL_checkstring(L, 2);
  list = regex::scan(string, regex);
  OStringList **ptr = (OStringList **)lua_newuserdata(L, sizeof(OStringList*));
  *ptr = list;
  luaL_getmetatable(L, "OStringList.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int lua_oberon_regex_split( lua_State *L ) {
  const char  * string = luaL_checkstring(L, 1);
  const char  * regex  = luaL_checkstring(L, 2);
  OStringList * list   = regex::split(string, regex);
  OStringList **ptr = (OStringList **)lua_newuserdata(L, sizeof(OStringList*));
  *ptr = list;
  luaL_getmetatable(L, "OStringList.metatable");
  lua_setmetatable(L, -2);

  return 1;
}


extern "C" {
  int luaopen_oberon_regex( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"ematch",  lua_oberon_regex_ematch},
      {"index",   lua_oberon_regex_index},
      {"match",   lua_oberon_regex_match},
      {"replace", lua_oberon_regex_replace},
      {"split",   lua_oberon_regex_split},
      {"scan",    lua_oberon_regex_scan},
      {NULL, NULL}
    };
    luaL_register(L, "regex", Map);
    return 1;
  }
}
