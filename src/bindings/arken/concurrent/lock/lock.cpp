// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/lock.h>

using arken::Lock;

/**
 * checkLock
 */

Lock *
checkLock( lua_State *L ) {
  return *(Lock **) luaL_checkudata(L, 1, "arken.concurrent.Lock.metatable");
}

/**
 * ClassMethods
 */

static int
arken_concurrent_Lock_new( lua_State *L ) {
  const char *str = (char *) luaL_checkstring(L, 1);
  Lock **ptr = (Lock **)lua_newuserdata(L, sizeof(Lock*));
  *ptr = new Lock(Lock(str));
  luaL_getmetatable(L, "arken.concurrent.Lock.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_concurrent_Lock[] = {
  {"new", arken_concurrent_Lock_new},
  {NULL, NULL}
};

void static
register_arken_concurrent_Lock( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.Lock");
  luaL_register(L, NULL, arken_concurrent_Lock);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_concurrent_Lock_gc( lua_State *L ) {
  Lock *udata = checkLock( L );
  delete udata;
  return 0;
}

static int
arken_concurrent_Lock_enable( lua_State *L ) {
  Lock * udata  = checkLock( L );
  udata->enable();
  return 0;
}

static int
arken_concurrent_Lock_disable( lua_State *L ) {
  Lock * udata  = checkLock( L );
  udata->disable();
  return 0;
}

static const
luaL_reg arken_concurrent_Lock_metatable[] = {
  {"enable",  arken_concurrent_Lock_enable},
  {"disable", arken_concurrent_Lock_disable},
  {"__gc",    arken_concurrent_Lock_gc},
  {NULL, NULL}
};

void static
register_arken_concurrent_Lock_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.Lock.metatable");
  luaL_register(L, NULL, arken_concurrent_Lock_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_Lock( lua_State *L ) {
    register_arken_concurrent_Lock_metatable(L);
    register_arken_concurrent_Lock(L);
    return 1;
  }
}
