// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/compress/zip.h>

using arken::compress::zip;

/**
 * ClassMethods
 */

static int
arken_compress_zip_descompress( lua_State *L ) {
  const char * filename = luaL_checkstring(L, 1);
  const char * output   = 0;
  if(lua_gettop(L) == 2) { // number of arguments
    output = luaL_checkstring(L, 2);
  }
  lua_pushboolean(L, zip::decompress(filename, output) );
  return 1;
}

static const luaL_reg arken_compress_zip_methods[] = {
  {"decompress", arken_compress_zip_descompress},
  {NULL, NULL}
};

void static
register_arken_compress_zip( lua_State *L ) {
  luaL_newmetatable(L, "arken.compress.zip");
  luaL_register(L, NULL, arken_compress_zip_methods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_compress_zip( lua_State *L ) {
    register_arken_compress_zip(L);
    return 1;
  }
}
