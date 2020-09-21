// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>


using arken::Lock;

/**
 * checkLock
 */

Lock *
checkLock( lua_State *L ) {
  return *(Lock **) luaL_checkudata(L, 1, "Lock.metatable");
}

/**
 * ClassMethods
 */

static int
arken_LockClassMethodNew( lua_State *L ) {
  const char *str = (char *) luaL_checkstring(L, 1);
  Lock **ptr = (Lock **)lua_newuserdata(L, sizeof(Lock*));
  *ptr = new Lock(Lock(str));
  luaL_getmetatable(L, "Lock.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg LockClassMethods[] = {
  {"new", arken_LockClassMethodNew},
  {NULL, NULL}
};

void static
registerLockClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "Lock");
  luaL_register(L, NULL, LockClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_LockInstanceMethodDestruct( lua_State *L ) {
  Lock *udata = checkLock( L );
  delete udata;
  return 0;
}

static int
arken_LockInstanceMethodEnable( lua_State *L ) {
  Lock * udata  = checkLock( L );
  udata->enable();
  return 0;
}

static int
arken_LockInstanceMethodDisable( lua_State *L ) {
  Lock * udata  = checkLock( L );
  udata->disable();
  return 0;
}

static const
luaL_reg LockInstanceMethods[] = {
  {"enable",  arken_LockInstanceMethodEnable},
  {"disable", arken_LockInstanceMethodDisable},
  {"__gc", arken_LockInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerLockInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "Lock.metatable");
  luaL_register(L, NULL, LockInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_Lock( lua_State *L ) {
    registerLockInstanceMethods(L);
    registerLockClassMethods(L);
    return 1;
  }
}
