// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>

using List = arken::string::List;
using arken::regex;

static int arken_regex_match( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * regex  = luaL_checkstring(L, 2);
  bool result = regex::match(string, regex);
  lua_pushboolean( L, result );
  return 1;
}

static int arken_regex_ematch( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * regex  = luaL_checkstring(L, 2);
  bool result = regex::ematch(string, regex);
  lua_pushboolean( L, result );
  return 1;
}

static int arken_regex_index( lua_State *L ) {
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

static int arken_regex_replace( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * regex  = luaL_checkstring(L, 2);
  const char * after  = luaL_checkstring(L, 3);
  char * result = regex::replace(string, regex, after);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int arken_regex_scan( lua_State *L ) {
  List * list;
  const char  * string = luaL_checkstring(L, 1);
  const char  * regex  = luaL_checkstring(L, 2);
  list = regex::scan(string, regex);
  List **ptr = (List **)lua_newuserdata(L, sizeof(List*));
  *ptr = list;
  luaL_getmetatable(L, "arken.string.List.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int arken_regex_split( lua_State *L ) {
  const char  * string = luaL_checkstring(L, 1);
  const char  * regex  = luaL_checkstring(L, 2);
  List * list   = regex::split(string, regex);
  List **ptr = (List **)lua_newuserdata(L, sizeof(List*));
  *ptr = list;
  luaL_getmetatable(L, "arken.string.List.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static void
register_arken_regex( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"ematch",  arken_regex_ematch},
    {"index",   arken_regex_index},
    {"match",   arken_regex_match},
    {"replace", arken_regex_replace},
    {"split",   arken_regex_split},
    {"scan",    arken_regex_scan},
    {NULL, NULL}
  };
  luaL_newmetatable(L, "regex");
  luaL_register(L, NULL, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_regex( lua_State *L ) {
    register_arken_regex( L ); 
    return 1;
  }
  int luaopen_arken_regex( lua_State *L ) {
    register_arken_regex( L ); 
    return 1;
  }
}
