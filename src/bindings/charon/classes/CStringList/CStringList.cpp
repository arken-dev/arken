// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <CStringList>

/**
 * checkCStringList
 */

CStringList *
checkCStringList( lua_State *L ) {
  return *(CStringList **) luaL_checkudata(L, 1, "CStringList.metatable");
}

/**
 * ClassMethods
 */

static int
lua_CStringListClassMethodNew( lua_State *L ) {
  CStringList **ptr = (CStringList **)lua_newuserdata(L, sizeof(CStringList*));
  *ptr= new CStringList(CStringList());
  luaL_getmetatable(L, "CStringList.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg CStringListClassMethods[] = {
  {"new", lua_CStringListClassMethodNew},
  {NULL, NULL}
};

void static
registerCStringListClassMethods( lua_State *L ) {
  luaL_register(L, "CStringList", CStringListClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_CStringListInstanceMethodDestruct( lua_State *L ) {
  CStringList *udata = checkCStringList( L );
  delete udata;
  return 0;
}

static int
lua_CStringListInstanceMethodAt( lua_State *L ) {
  CStringList * udata  = checkCStringList( L );
  int pos = luaL_checkint(L, 2);
  int len;
  const char * str = udata->at(pos-1, &len);
  if( str == 0 ) {
    lua_pushnil(L);
  } else {
    lua_pushlstring(L, str, len);
  }
  return 1;
}

static int
lua_CStringListInstanceMethodFirst( lua_State *L ) {
  CStringList * udata  = checkCStringList( L );
  lua_pushstring(L, udata->first());
  return 1;
}

static int
lua_CStringListInstanceMethodLast( lua_State *L ) {
  CStringList * udata  = checkCStringList( L );
  lua_pushstring(L, udata->last());
  return 1;
}

static int
lua_CStringListInstanceMethodReplace( lua_State *L ) {
  CStringList * udata  = checkCStringList( L );
  int pos = luaL_checkint(L, 2);
  const char * value = luaL_checkstring(L, 3);
  udata->replace(pos, value);
  return 0;
}

static int
lua_CStringListInstanceMethodJoin( lua_State *L ) {
  CStringList * udata     = checkCStringList( L );
  const char  * separator = luaL_checkstring(L, 2);
  char * result = udata->join(separator);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}


static int
lua_CStringListInstanceMethodAppend( lua_State *L ) {
  CStringList * udata  = checkCStringList( L );
  const char * value = luaL_checkstring(L, 2);
  udata->append(value);
  lua_pushvalue(L, -2);
  return 1;
}

static int
lua_CStringListInstanceMethodSize( lua_State *L ) {
  CStringList * udata  = checkCStringList( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static const
luaL_reg CStringListInstanceMethods[] = {
  {"append", lua_CStringListInstanceMethodAppend},
  {"at", lua_CStringListInstanceMethodAt},
  {"first", lua_CStringListInstanceMethodFirst},
  {"last", lua_CStringListInstanceMethodLast},
  {"join", lua_CStringListInstanceMethodJoin},
  {"size", lua_CStringListInstanceMethodSize},
  {"replace", lua_CStringListInstanceMethodReplace},
  {"__gc", lua_CStringListInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerCStringListInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "CStringList.metatable");
  luaL_register(L, NULL, CStringListInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_CStringList( lua_State *L ) {
    registerCStringListClassMethods(L);
    registerCStringListInstanceMethods(L);
    return 1;
  }
}
