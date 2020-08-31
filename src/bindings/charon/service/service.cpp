// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/service>

using charon::service;

static int
charon_service_loop(lua_State * L) {
  lua_getglobal(L, "__charon_service");
  service * srv = (service *) lua_touserdata(L, -1);
  int secs = luaL_checkint(L, 1);
  lua_pushboolean(L, srv->loop(secs, L));
  return 1;
}

static int
charon_service_quit(lua_State * L) {
  lua_getglobal(L, "__charon_service");
  service * srv = (service *) lua_touserdata(L, -1);
  srv->quit();
  return 0;
}

static int
charon_service_exit(lua_State * L) {
  lua_getglobal(L, "__charon_service");
  service * srv = (service *) lua_touserdata(L, -1);
  srv->exit();
  return 0;
}

static int
charon_service_start(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  char * uuid = service::start( fileName );
  lua_pushstring(L, uuid);
  delete[] uuid;
  return 1;
}

static int
charon_service_load(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  service::load( fileName );
  return 0;
}

static void
register_arken_service( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"quit",    charon_service_quit},
    {"exit",    charon_service_exit},
    {"loop",    charon_service_loop},
    {"start",   charon_service_start},
    {"load",    charon_service_load},
    {NULL, NULL}
  };
  luaL_newmetatable(L, "service");
  luaL_register(L, NULL, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_service( lua_State *L ) {
    register_arken_service( L );
    return 1;
  }
  int luaopen_charon_service( lua_State *L ) {
    register_arken_service( L );
    return 1;
  }
}
