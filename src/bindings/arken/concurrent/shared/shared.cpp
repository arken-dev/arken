// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/shared.h>

using Shared = arken::concurrent::Shared;
using string = arken::string;

/**
 * checkShared
 */

Shared *
checkShared( lua_State *L ) {
  return *(Shared **) luaL_checkudata(L, 1, "arken.concurrent.Shared.metatable");
}

/**
 * InstanceMethods
 */

static int
arken_concurrent_shared_instance_setNumber( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  double value = luaL_checknumber(L, 3);
  shr->setNumber(key, value);
  return 0;
}

static int
arken_concurrent_shared_instance_getNumber( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  lua_pushnumber(L, shr->getNumber(key));
  return 1;
}

static int
arken_concurrent_shared_instance_increment( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  double value = luaL_checknumber(L, 3);
  lua_pushnumber(L, shr->increment(key, value));
  return 1;
}

static int
arken_concurrent_shared_instance_setString( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  size_t len;
  const char * value = luaL_checklstring(L, 3, &len);
  shr->setString(key, string(value, len));
  return 0;
}

static int
arken_concurrent_shared_instance_getString( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  string str = shr->getString(key);
  lua_pushlstring(L, str.data(), str.size());
  return 1;
}

static int
arken_concurrent_shared_instance_append( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  size_t len;
  string value = luaL_checklstring(L, 3, &len);
  string str = shr->append(key, string(value, len));
  lua_pushlstring(L, str.data(), str.size());
  return 1;
}

static int
arken_concurrent_shared_instance_setBool( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  bool value = lua_toboolean(L, 3);
  shr->setBool(key, value);
  return 0;
}

static int
arken_concurrent_shared_instance_getBool( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  bool value = shr->getBool(key);
  lua_pushboolean(L, value);
  return 1;
}

static int
arken_concurrent_shared_instance_toggle( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  bool value = shr->toggle(key);
  lua_pushboolean(L, value);
  return 1;
}

static int
arken_concurrent_shared_instance_destruct( lua_State *L ) {
  Shared * shr = checkShared( L );
  delete shr;
  return 0;
}

static const
luaL_reg SharedInstanceMethods[] = {
  {"setNumber", arken_concurrent_shared_instance_setNumber},
  {"getNumber", arken_concurrent_shared_instance_getNumber},
  {"increment", arken_concurrent_shared_instance_increment},
  {"setString", arken_concurrent_shared_instance_setString},
  {"getString", arken_concurrent_shared_instance_getString},
  {"append",    arken_concurrent_shared_instance_append},
  {"setBool",   arken_concurrent_shared_instance_setBool},
  {"getBool",   arken_concurrent_shared_instance_getBool},
  {"toggle",    arken_concurrent_shared_instance_toggle},
  {"__gc",      arken_concurrent_shared_instance_destruct},
  {NULL, NULL}
};

void static
registerSharedInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.Shared.metatable");
  luaL_register(L, NULL, SharedInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_Shared( lua_State *L ) {
    registerSharedInstanceMethods(L);
    return 1;
  }
}