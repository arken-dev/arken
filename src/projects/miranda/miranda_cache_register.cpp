// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include "mirandastate.h"

extern "C" {
#include <json.h>
}

char * json_lock_encode(lua_State *l);
void json_lock_decode(lua_State *l, const char * data);

static int
miranda_cache_insert(lua_State *L) {
  const char * key   = luaL_checkstring(L, 1);
  int expires = -1;
  if(lua_gettop(L) == 3) { /* número de argumentos */
    expires = lua_tointeger(L, 3);
    lua_remove(L, 3);
  }
  char * value = json_lock_encode(L);
  MirandaState::insert(key, value, expires);
  return 0;
}

static int
miranda_cache_value(lua_State *L) {
  const char * key   = luaL_checkstring(L, 1);
  const char * value = MirandaState::value(key);
  if( value == 0 ) {
    lua_pushnil(L);
  } else {
    json_lock_decode(L, value);
  }
  return 1;
}

static int
miranda_cache_remove(lua_State *L) {
  const char * key = luaL_checkstring(L, 1);
  lua_pushinteger(L, MirandaState::remove(key));
  return 0;
}

void
miranda_cache_register(lua_State * L) {
  static const luaL_reg Map[] = {
    {"insert", miranda_cache_insert},
    {"remove",  miranda_cache_remove},
    {"value",  miranda_cache_value},
    {NULL, NULL}
  };

  luaL_register(L, "cache", Map);
}
