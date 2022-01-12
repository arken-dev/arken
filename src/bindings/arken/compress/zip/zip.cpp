// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/compress/zip.h>

using arken::compress::Zip;

/**
 * checkZip
 */

Zip *
checkZip( lua_State *L ) {
  return *(Zip **) luaL_checkudata(L, 1, "arken.compress.Zip.metatable");
}


/**
 * ClassMethods
 */

static int
arken_compress_Zip_extract( lua_State *L ) {
  const char * filename = luaL_checkstring(L, 1);
  const char * output   = 0;
  if(lua_gettop(L) == 2) { // number of arguments
    output = luaL_checkstring(L, 2);
  }
  lua_pushboolean(L, Zip::extract(filename, output) );
  return 1;
}

static int
arken_compress_Zip_new( lua_State *L ) {
  const char * filename = luaL_checkstring(L, 1);
  Zip **ptr = (Zip **)lua_newuserdata(L, sizeof(Zip*));
  *ptr = new Zip(filename);
  luaL_getmetatable(L, "arken.compress.Zip.metatable");
  lua_setmetatable(L, -2);

  return 1;
}


static const luaL_reg arken_compress_Zip[] = {
  {"extract", arken_compress_Zip_extract},
  {"new",     arken_compress_Zip_new},
  {NULL, NULL}
};

void static
register_arken_compress_Zip( lua_State *L ) {
  luaL_newmetatable(L, "arken.compress.Zip");
  luaL_register(L, NULL, arken_compress_Zip);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_compress_Zip_gc( lua_State *L ) {
  Zip *udata = checkZip( L );
  delete udata;
  return 0;
}

static int
arken_compress_Zip_addFile( lua_State *L ) {
  Zip * udata  = checkZip( L );
  const char * filename = luaL_checkstring(L, 2);
  udata->addFile(filename);
  return 0;
}

static int
arken_compress_Zip_addBuffer( lua_State *L ) {
  Zip * udata  = checkZip( L );
  const char * filename = luaL_checkstring(L, 2);
  size_t len;
  const char * buf = luaL_checklstring(L, 3, &len);
  udata->addBuffer(filename, buf, len);
  return 0;
}

static int
arken_compress_Zip_save( lua_State *L ) {
  Zip * udata  = checkZip( L );
  udata->save();
  return 0;
}

static const
luaL_reg arken_compress_Zip_metatable[] = {
  {"addFile",   arken_compress_Zip_addFile},
  {"addBuffer", arken_compress_Zip_addBuffer},
  {"save",      arken_compress_Zip_save},
  {"__gc",      arken_compress_Zip_gc},
  {NULL, NULL}
};

void static
register_arken_compress_Zip_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.compress.Zip.metatable");
  luaL_register(L, NULL, arken_compress_Zip_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_compress_Zip( lua_State *L ) {
    register_arken_compress_Zip_metatable(L);
    register_arken_compress_Zip(L);
    return 1;
  }
}
