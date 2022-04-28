// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/log>

using arken::Log;

/**
 * checkLog
 */

Log *
checkLog( lua_State *L ) {
  return *static_cast<Log **>(luaL_checkudata(L, 1, "arken.Log.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_Log_new( lua_State *L ) {
  const char *str = luaL_checkstring(L, 1);
  int max = -1;
  if(lua_gettop(L) == 2) { // number of arguments
    max = luaL_checkinteger(L, 2);
  }

  auto ptr = static_cast<Log **>(lua_newuserdata(L, sizeof(Log*)));
  *ptr = new Log(Log(str, max));
  luaL_getmetatable(L, "arken.Log.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_Log[] = {
  {"new", arken_Log_new},
  {nullptr, nullptr}
};

void static
register_arken_Log( lua_State *L ) {
  luaL_newmetatable(L, "arken.Log");
  luaL_register(L, nullptr, arken_Log);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_Log_gc( lua_State *L ) {
  Log *udata = checkLog( L );
  delete udata;
  return 0;
}

static int
arken_Log_append( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->append(append);
  return 0;
}

static int
arken_Log_dump( lua_State *L ) {
  Log *udata  = checkLog( L );
  udata->dump();
  return 0;
}

static int
arken_Log_log( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * level  = lua_tostring(L, 2);
  const char * append = lua_tostring(L, 3);
  udata->log(level, append);
  return 0;
}

static int
arken_Log_trace( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->trace(append);
  return 0;
}

static int
arken_Log_debug( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->debug(append);
  return 0;
}

static int
arken_Log_info( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->info(append);
  return 0;
}

static int
arken_Log_warn( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->warn(append);
  return 0;
}

static int
arken_Log_error( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->error(append);
  return 0;
}

static int
arken_Log_fatal( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->fatal(append);
  return 0;
}

static const
luaL_reg arken_Log_metatable[] = {
  {"append", arken_Log_append},
  {"dump",   arken_Log_dump},
  {"log",    arken_Log_log},
  {"trace",  arken_Log_trace},
  {"debug",  arken_Log_debug},
  {"info",   arken_Log_info},
  {"warn",   arken_Log_warn},
  {"error",  arken_Log_error},
  {"fatal",  arken_Log_fatal},
  {"__gc",   arken_Log_gc},
  {nullptr, nullptr}
};

void static
register_arken_Log_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.Log.metatable");
  luaL_register(L, nullptr, arken_Log_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_Log( lua_State *L ) {
    register_arken_Log_metatable(L);
    register_arken_Log(L);
    return 1;
  }
}
