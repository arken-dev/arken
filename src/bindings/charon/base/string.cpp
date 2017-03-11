// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <iostream>

using charon::ByteArray;
using charon::ByteArrayList;

static int lua_charon_string_append( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  char *result = string::append(string, str);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_camelCase( lua_State *L ) {
  char * result;
  const char *string = luaL_checkstring(L, 1);
  bool  flag = false;

  if(lua_gettop(L) == 2) { /* número de argumentos */
    flag =  lua_toboolean(L, 2);
  }

  result = string::camelCase(string, flag);

  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_capitalize( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char *result = string::capitalize(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_center( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int size           = luaL_checkinteger(L, 2);
  const char *pad    = luaL_checkstring(L, 3);
  char *result       = string::center(string, size, pad);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_chop( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int n =  luaL_checkinteger(L, 2);
  char * result = string::chop(string, n);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_contains( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  bool result        = string::contains(string, str);
  lua_pushboolean(L, result);
  return 1;
}

static int lua_charon_string_endsWith( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  bool result        = string::endsWith(string, str);
  lua_pushboolean(L, result);
  return 1;
}

static int lua_charon_string_escape( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char * result      = string::escape(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_escapeHtml( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char * result      = string::escapeHtml(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_indexOf( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  int result = string::indexOf(string, str);
  if( result > -1 ) {
    result ++;
  }
  lua_pushnumber(L, result);
  return 1;
}

static int lua_charon_string_insert( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int len            = luaL_checkinteger(L, 2);
  const char *ba     = luaL_checkstring(L, 3);
  char *result = string::insert(string, len-1, ba);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_lastIndexOf( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  int result = string::lastIndexOf(string, str);
  if( result > -1 ) {
    result ++;
  }
  lua_pushnumber(L, result);
  return 1;
}

static int lua_charon_string_left( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    len    =  luaL_checkinteger(L, 2);
  char * result = string::left(string, len);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_mid( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int pos      =  luaL_checkinteger(L, 2);
  int len;
  if(lua_gettop(L) == 3) { /* número de argumentos */
    len =  luaL_checkinteger(L, 3);
  } else {
    len = -1;
  }
  char *result = string::mid(string, pos-1, len);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_normalize( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::normalize(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_leftJustified( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int size           = luaL_checkinteger(L, 2);
  const char *pad    = luaL_checkstring(L, 3);
  char *result       = string::leftJustified(string, size, pad);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_rightJustified( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int size           = luaL_checkinteger(L, 2);
  const char *pad    = luaL_checkstring(L, 3);
  char *result       = string::rightJustified(string, size, pad);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_repeated( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    times  =  luaL_checkinteger(L, 2);
  char * result = string::repeated(string, times);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_right( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    len    =  luaL_checkinteger(L, 2);
  char * result = string::right(string, len);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_simplified( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result = string::simplified(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_startsWith( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * ba     = luaL_checkstring(L, 2);
  bool result         = string::startsWith(string, ba);
  lua_pushboolean(L, result);  /* push result */
  return 1;
}

static int lua_charon_string_suffix( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char chr = '.';
  char * result;
  if(lua_gettop(L) == 2) { /* número de argumentos */
    chr =  luaL_checkstring(L, 2)[0];
  }
  result = string::suffix(string, chr);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_split( lua_State *L ) {
  size_t len;
  const char  * string  = luaL_checklstring(L, 1, &len);
  const char  * pattern = luaL_checkstring(L, 2);
  ByteArrayList * list    = string::split(string, len, pattern);
  ByteArrayList **ptr = (ByteArrayList **)lua_newuserdata(L, sizeof(ByteArrayList*));
  *ptr = list;
  luaL_getmetatable(L, "ByteArrayList.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int lua_charon_string_replace( lua_State *L ) {
  size_t len1, len2;
  const char * string = luaL_checkstring(L, 1);
  const char * before = luaL_checklstring(L, 2, &len1);
  const char * after  = luaL_checklstring(L, 3, &len2);
  char * result;
  if( len1 == 1 && len2 == 1) {
    result = string::replace(string, before[0], after[0]);
  } else {
    result = string::replace(string, before, after);
  }
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_trimmed( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::trimmed(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_leftTrimmed( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::leftTrimmed(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_rightTrimmed( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::rightTrimmed(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int lua_charon_string_truncate( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int pos = luaL_checkinteger(L, 2);
  const char * omission;
  char separator;

  if(lua_gettop(L) >= 3) { /* número de argumentos */
    omission = lua_tostring(L, 3);
  } else {
    omission = "...";
  }

  if(lua_gettop(L) == 4) { /* número de argumentos */
    separator = lua_tostring(L, 4)[0];
  } else {
    separator = ' ';
  }

  char  * result = string::truncate(string, pos, omission, separator);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int lua_charon_string_underscore( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char      * result = string::underscore(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

int luaopen_charon_string( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"append",         lua_charon_string_append},
    {"camelCase",      lua_charon_string_camelCase},
    {"capitalize",     lua_charon_string_capitalize},
    {"center",         lua_charon_string_center},
    {"contains",       lua_charon_string_contains},
    {"chop",           lua_charon_string_chop},
    {"endsWith",       lua_charon_string_endsWith},
    {"escape",         lua_charon_string_escape},
    {"escapeHtml",     lua_charon_string_escapeHtml},
    {"indexOf",        lua_charon_string_indexOf},
    {"insert",         lua_charon_string_insert},
    {"left",           lua_charon_string_left},
    {"lastIndexOf",    lua_charon_string_lastIndexOf},
    {"mid",            lua_charon_string_mid},
    {"normalize",      lua_charon_string_normalize},
    {"leftJustified",  lua_charon_string_leftJustified},
    {"rightJustified", lua_charon_string_rightJustified},
    {"simplified",     lua_charon_string_simplified},
    {"repeated",       lua_charon_string_repeated},
    {"replace",        lua_charon_string_replace},
    {"right",          lua_charon_string_right},
    {"startsWith",     lua_charon_string_startsWith},
    {"split",          lua_charon_string_split},
    {"suffix",         lua_charon_string_suffix},
    {"trimmed",        lua_charon_string_trimmed},
    {"leftTrimmed",    lua_charon_string_leftTrimmed},
    {"rightTrimmed",   lua_charon_string_rightTrimmed},
    {"truncate",       lua_charon_string_truncate},
    {"underscore",     lua_charon_string_underscore},
    {NULL, NULL}
  };
  luaL_register(L, "string", Map);
  return 1;
}
