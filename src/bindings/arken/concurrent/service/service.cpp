// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/mvm>
#include <arken/concurrent/service.h>

using service = arken::concurrent::service;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

service *
checkService ( lua_State *L ) {
  return *(service **) luaL_checkudata(L, 1, "arken.concurrent.service.metatable");
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_service_start(lua_State *L) {
  bool purge = false;
  const char * fileName = luaL_checkstring(L, 1);
  char * params;

  if(lua_gettop(L) == 1) { /* número de argumentos */
    params = new char[3]{'{','}','\0'};
  } else {
    if(lua_gettop(L) == 3) { // number of arguments
      purge = lua_toboolean(L, 3);
      lua_settop(L, 2);
    }
    params = json_lock_encode(L);
  }

  service::start( fileName, params, purge );

  delete params;

  return 0;
}

static int
arken_service_wait(lua_State *L) {
  mvm::wait();
  return 0;
}

static const luaL_reg TaskClassMethods[] = {
  {"start", arken_service_start},
  {"wait",  arken_service_wait},
  {NULL, NULL}
};

void static
registerServiceClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.service");
  luaL_register(L, NULL, TaskClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_service_instance_method_loop( lua_State *L ) {
  service * srv = checkService( L );
  int secs = luaL_checkint(L, 2);
  lua_pushboolean(L, srv->loop( secs ));
  return 1;
}

static const
luaL_reg ServiceInstanceMethods[] = {
  {"loop",     arken_concurrent_service_instance_method_loop},
  {NULL, NULL}
};

void static
registerServiceInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.service.metatable");
  luaL_register(L, NULL, ServiceInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_service( lua_State *L ) {
    registerServiceInstanceMethods(L);
    registerServiceClassMethods(L);
    return 1;
  }
}
