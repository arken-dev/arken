// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/lock.h>

using arken::concurrent::Lock;
using Shared = arken::mvm::Shared;

/**
 * checkLock
 */

Lock *
checkLock( lua_State *L ) {
  return *static_cast<Lock **>(luaL_checkudata(L, 1, "arken.concurrent.Lock.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_concurrent_Lock_new( lua_State *L ) {
  const char *str = luaL_checkstring(L, 1);
  bool reentrant = false;
  if( lua_gettop(L) == 2 ) { /* número de argumentos */
    reentrant = lua_toboolean(L, 2);
  }

  auto ptr = static_cast<Lock **>(lua_newuserdata(L, sizeof(Lock*)));
  *ptr = new Lock(Lock(str, reentrant));
  luaL_getmetatable(L, "arken.concurrent.Lock.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_concurrent_Lock[] = {
  {"new", arken_concurrent_Lock_new},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_Lock( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.Lock");
  luaL_register(L, nullptr, arken_concurrent_Lock);
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

  try {
    udata->enable();
  } catch (const char* msg) {
    lua_pushstring(L, msg);
    lua_error(L);
  }

  return 0;
}

static int
arken_concurrent_Lock_disable( lua_State *L ) {
  Lock * udata  = checkLock( L );

  try {
    udata->disable();
  } catch (const char* msg) {
    lua_pushstring(L, msg);
    lua_error(L);
  }

  return 0;
}

static int
arken_concurrent_Lock_call( lua_State *L ) {
  Lock * udata  = checkLock( L );
  try {
    udata->call( L );
  } catch (const char* msg) {
    lua_pushstring(L, msg);
    lua_error(L);
  }

  return 0;
}

static int
arken_concurrent_Lock_pcall( lua_State *L ) {
  Lock * udata  = checkLock( L );
  return udata->pcall( L );
}


static int
arken_concurrent_Lock_shared( lua_State *L ) {
  Lock * udata  = checkLock( L );
  int rv;
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.mvm");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  auto ptr = static_cast<Shared **>(lua_newuserdata(L, sizeof(Shared*)));
  *ptr = new Shared(udata->shared());
  luaL_getmetatable(L, "arken.mvm.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const
luaL_reg arken_concurrent_Lock_metatable[] = {
  {"enable",  arken_concurrent_Lock_enable},
  {"disable", arken_concurrent_Lock_disable},
  {"call",    arken_concurrent_Lock_call},
  {"pcall",   arken_concurrent_Lock_pcall},
  {"shared",  arken_concurrent_Lock_shared},
  {"__gc",    arken_concurrent_Lock_gc},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_Lock_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.Lock.metatable");
  luaL_register(L, nullptr, arken_concurrent_Lock_metatable);
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
