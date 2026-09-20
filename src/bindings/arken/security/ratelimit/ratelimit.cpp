// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/security/ratelimit.h>

using arken::security::RateLimit;

/**
 * checkRateLimit
 */

RateLimit *
checkRateLimit( lua_State *L ) {
  return *static_cast<RateLimit **>(luaL_checkudata(L, 1, "arken.security.RateLimit.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_security_RateLimit_new( lua_State *L ) {
  unsigned limit   = static_cast<unsigned>(luaL_checkinteger(L, 1));
  unsigned seconds = 60;
  if( lua_gettop(L) >= 2 ) {
    seconds = static_cast<unsigned>(luaL_checkinteger(L, 2));
  }
  auto ptr = static_cast<RateLimit **>(lua_newuserdata(L, sizeof(RateLimit*)));
  *ptr = new RateLimit(limit, seconds);
  luaL_getmetatable(L, "arken.security.RateLimit.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const luaL_reg arken_security_RateLimit[] = {
  {"new", arken_security_RateLimit_new},
  {nullptr, nullptr}
};

void static
register_arken_security_RateLimit( lua_State *L ) {
  luaL_newmetatable(L, "arken.security.RateLimit");
  luaL_register(L, nullptr, arken_security_RateLimit);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_security_RateLimit_gc( lua_State *L ) {
  RateLimit *udata = checkRateLimit( L );
  delete udata;
  return 0;
}

static int
arken_security_RateLimit_count( lua_State *L ) {
  RateLimit * udata = checkRateLimit( L );
  const char * ip   = luaL_checkstring(L, 2);
  lua_pushboolean(L, udata->count(ip));
  return 1;
}

static int
arken_security_RateLimit_clear( lua_State *L ) {
  RateLimit * udata = checkRateLimit( L );
  const char * ip   = luaL_checkstring(L, 2);
  udata->clear(ip);
  return 0;
}

static int
arken_security_RateLimit_gcIdle( lua_State *L ) {
  RateLimit * udata = checkRateLimit( L );
  unsigned idle     = static_cast<unsigned>(luaL_checkinteger(L, 2));
  udata->gc(idle);
  return 0;
}

static const
luaL_reg arken_security_RateLimit_metatable[] = {
  {"count", arken_security_RateLimit_count},
  {"clear", arken_security_RateLimit_clear},
  {"gc",    arken_security_RateLimit_gcIdle},
  {"__gc",  arken_security_RateLimit_gc},
  {nullptr, nullptr}
};

void static
register_arken_security_RateLimit_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.security.RateLimit.metatable");
  luaL_register(L, nullptr, arken_security_RateLimit_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_security_RateLimit( lua_State *L ) {
    register_arken_security_RateLimit_metatable(L);
    register_arken_security_RateLimit(L);
    return 1;
  }
}
