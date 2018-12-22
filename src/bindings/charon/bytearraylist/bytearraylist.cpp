// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <QtCore>

/**
 * checkByteArrayList
 */

ByteArrayList *
checkByteArrayList( lua_State *L ) {
  return *(ByteArrayList **) luaL_checkudata(L, 1, "ByteArrayList.metatable");
}

/**
 * ClassMethods
 */

static int
charon_ByteArrayListClassMethodNew( lua_State *L ) {
  ByteArrayList **ptr = (ByteArrayList **)lua_newuserdata(L, sizeof(ByteArrayList*));
  *ptr= new ByteArrayList(ByteArrayList());
  luaL_getmetatable(L, "ByteArrayList.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg ByteArrayListClassMethods[] = {
  {"new", charon_ByteArrayListClassMethodNew},
  {NULL, NULL}
};

void static
registerByteArrayListClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "ByteArrayList");
  luaL_register(L, NULL, ByteArrayListClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
charon_ByteArrayListInstanceMethodDestruct( lua_State *L ) {
  ByteArrayList *udata = checkByteArrayList( L );
  delete udata;
  return 0;
}

static int
charon_ByteArrayListInstanceMethodAt( lua_State *L ) {
  ByteArrayList * udata  = checkByteArrayList( L );
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
charon_byte_array_list_each( lua_State *L ) {
  int i = lua_upvalueindex(1);
  luaL_checktype(L, i, LUA_TLIGHTUSERDATA);
  ByteArrayList * ba  = (ByteArrayList *) lua_touserdata(L, i);
  const char * result = ba->each();
  if( result == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, result);
  }
  return 1;
}

static int
charon_ByteArrayListInstanceMethodEach( lua_State *L ) {
  ByteArrayList * udata  = checkByteArrayList( L );
  lua_pushlightuserdata(L, udata);
  lua_pushcclosure(L, charon_byte_array_list_each, 1);
  return 1;
}

static int
charon_ByteArrayListInstanceMethodFirst( lua_State *L ) {
  ByteArrayList * udata  = checkByteArrayList( L );
  lua_pushstring(L, udata->first());
  return 1;
}

static int
charon_ByteArrayListInstanceMethodLast( lua_State *L ) {
  ByteArrayList * udata  = checkByteArrayList( L );
  lua_pushstring(L, udata->last());
  return 1;
}

static int
charon_ByteArrayListInstanceMethodReplace( lua_State *L ) {
  ByteArrayList * udata  = checkByteArrayList( L );
  int pos = luaL_checkint(L, 2);
  const char * value = luaL_checkstring(L, 3);
  udata->replace(pos, value);
  return 0;
}

static int
charon_ByteArrayListInstanceMethodJoin( lua_State *L ) {
  ByteArrayList * udata     = checkByteArrayList( L );
  const char  * separator = luaL_checkstring(L, 2);
  char * result = udata->join(separator);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}


static int
charon_ByteArrayListInstanceMethodAppend( lua_State *L ) {
  ByteArrayList * udata  = checkByteArrayList( L );
  const char * value = luaL_checkstring(L, 2);
  udata->append(value);
  lua_pushvalue(L, -2);
  return 1;
}

static int
charon_ByteArrayListInstanceMethodSize( lua_State *L ) {
  ByteArrayList * udata  = checkByteArrayList( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static const
luaL_reg ByteArrayListInstanceMethods[] = {
  {"append", charon_ByteArrayListInstanceMethodAppend},
  {"at", charon_ByteArrayListInstanceMethodAt},
  {"each",    charon_ByteArrayListInstanceMethodEach},
  {"first", charon_ByteArrayListInstanceMethodFirst},
  {"last", charon_ByteArrayListInstanceMethodLast},
  {"join", charon_ByteArrayListInstanceMethodJoin},
  {"size", charon_ByteArrayListInstanceMethodSize},
  {"replace", charon_ByteArrayListInstanceMethodReplace},
  {"__gc", charon_ByteArrayListInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerByteArrayListInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "ByteArrayList.metatable");
  luaL_register(L, NULL, ByteArrayListInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_charon_ByteArrayList( lua_State *L ) {
    registerByteArrayListInstanceMethods(L);
    registerByteArrayListClassMethods(L);
    return 1;
  }
}
