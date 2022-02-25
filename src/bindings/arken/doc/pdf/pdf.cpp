// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/doc/pdf.h>

using arken::doc::Pdf;

/**
 * checkPdf
 */

arken::doc::Pdf *
checkPdf( lua_State *L , int position = 1) {
  return *(arken::doc::Pdf **) luaL_checkudata(L, position, "arken.doc.Pdf.metatable");
}

/**
 * ClassMethods
 */

static int
arken_doc_Pdf_new( lua_State *L ) {
  Pdf **ptr  = (Pdf **)lua_newuserdata(L, sizeof(Pdf *));
  *ptr = new Pdf();

  luaL_getmetatable(L, "arken.doc.Pdf.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_doc_Pdf[] = {
  {"new", arken_doc_Pdf_new},
  {NULL, NULL}
};

void static
register_arken_doc_Pdf( lua_State *L ) {
  luaL_newmetatable(L, "arken.doc.Pdf");
  luaL_register(L, NULL, arken_doc_Pdf);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_doc_Pdf_text( lua_State *L ) {
  Pdf *ptr= checkPdf( L );
  int width  = luaL_checkinteger(L, 2);
  int height = luaL_checkinteger(L, 3);
  const char * text = lua_tostring(L, 4);

  ptr->text(width, height, text);
  return 0;
}

static int
arken_doc_Pdf_save( lua_State *L ) {
  Pdf *ptr = checkPdf( L );
  const char * path = lua_tostring(L, 2);
  ptr->save(path);

  return 0;
}

static int
arken_doc_Pdf_gc( lua_State *L ) {
  Pdf * ptr = checkPdf( L );
  delete ptr;
  return 0;
}

static const
luaL_reg arken_doc_Pdf_metatable[] = {
  {"text",     arken_doc_Pdf_text},
  {"setText",  arken_doc_Pdf_text},
  {"save",     arken_doc_Pdf_save},
  {"__gc",     arken_doc_Pdf_gc},
  {NULL, NULL}
};

void static
register_arken_doc_Pdf_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.doc.Pdf.metatable");
  luaL_register(L, NULL, arken_doc_Pdf_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_doc_Pdf( lua_State *L ) {
    register_arken_doc_Pdf_metatable(L);
    register_arken_doc_Pdf(L);
    return 1;
  }
}
