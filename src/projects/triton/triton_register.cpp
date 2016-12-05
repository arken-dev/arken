// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <OByteArray>
#include <QDebug>
#include <QThread>
#include <charon/helper>
#include <triton.h>

static int
triton_enqueue(lua_State *L) {
  const char * path = luaL_checkstring(L, 1);
  Triton::enqueue(path);
  return 0;
}

static int
triton_count(lua_State *L) {
  const char * label = luaL_checkstring(L, 1);
  Triton::count(label);
  return 0;
}

static int
triton_total(lua_State *L) {
  const char * label = luaL_checkstring(L, 1);
  lua_pushinteger(L, Triton::total(label));
  return 1;
}

static int
triton_append(lua_State *L) {
  const char * key    = luaL_checkstring(L, 1);
  const char * result = luaL_checkstring(L, 2);
  Triton::append(key, result);
  return 0;
}

static int
triton_result(lua_State *L) {
  const char * key = luaL_checkstring(L, 1);
  lua_pushstring(L, *Triton::result(key));
  return 1;
}


void
triton_register(lua_State * L) {
  static const luaL_reg Map[] = {
    {"enqueue",    triton_enqueue},
    {"count",      triton_count},
    {"total",      triton_total},
    {"append",     triton_append},
    {"result",     triton_result},
    {NULL, NULL}
  };

  luaL_register(L, "triton", Map);
}
