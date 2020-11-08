// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/image.h>

using arken::Image;

/**
 * checkImage
 */

Image *
checkImage( lua_State *L ) {
  return *(Image **) luaL_checkudata(L, 1, "Image.metatable");
}

/**
 * ClassMethods
 */

static int
arken_ImageClassMethodNew( lua_State *L ) {
  size_t len;
  const char *path = (char *) lua_tolstring(L, 1, &len);
  Image **ptr = (Image **)lua_newuserdata(L, sizeof(Image*));
  *ptr= new Image(Image(path));
  luaL_getmetatable(L, "Image.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg ImageClassMethods[] = {
  {"new", arken_ImageClassMethodNew},
  {NULL, NULL}
};

void static
registerImageClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "Image");
  luaL_register(L, NULL, ImageClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_ImageInstanceMethodSave( lua_State *L ) {
  Image *image = checkImage( L );
  const char * path= lua_tostring(L, 2);
  int quality = lua_tointeger(L, 3);
  image->save(path, quality);
  return 0;
}


static int
arken_ImageInstanceMethodDestruct( lua_State *L ) {
  Image *image = checkImage( L );
  delete image;
  return 0;
}

static const
luaL_reg ImageInstanceMethods[] = {
  {"save", arken_ImageInstanceMethodSave},
  {"__gc", arken_ImageInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerImageInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "Image.metatable");
  luaL_register(L, NULL, ImageInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_Image( lua_State *L ) {
    registerImageInstanceMethods(L);
    registerImageClassMethods(L);
    return 1;
  }
}
