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
  return *(Log **) luaL_checkudata(L, 1, "Log.metatable");
}

/**
 * ClassMethods
 */

static int
arken_LogClassMethodNew( lua_State *L ) {
  const char *str = (char *) luaL_checkstring(L, 1);
  int max = 1;
  if(lua_gettop(L) == 2) { // number of arguments
    max = luaL_checkinteger(L, 2);
  }

  Log **ptr = (Log **)lua_newuserdata(L, sizeof(Log*));
  *ptr = new Log(Log(str, max));
  luaL_getmetatable(L, "Log.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg LogClassMethods[] = {
  {"new", arken_LogClassMethodNew},
  {NULL, NULL}
};

void static
registerLogClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "Log");
  luaL_register(L, NULL, LogClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_LogInstanceMethodDestruct( lua_State *L ) {
  Log *udata = checkLog( L );
  delete udata;
  return 0;
}

static int
arken_LogInstanceMethodAppend( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->append(append);
  return 0;
}

static int
arken_LogInstanceMethodDump( lua_State *L ) {
  Log *udata  = checkLog( L );
  udata->dump();
  return 0;
}

static int
arken_LogInstanceMethodLog( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * level  = lua_tostring(L, 2);
  const char * append = lua_tostring(L, 3);
  udata->log(level, append);
  return 0;
}

static int
arken_LogInstanceMethodTrace( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->trace(append);
  return 0;
}

static int
arken_LogInstanceMethodDebug( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->debug(append);
  return 0;
}

static int
arken_LogInstanceMethodInfo( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->info(append);
  return 0;
}

static int
arken_LogInstanceMethodWarn( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->warn(append);
  return 0;
}

static int
arken_LogInstanceMethodError( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->error(append);
  return 0;
}

static int
arken_LogInstanceMethodFatal( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->fatal(append);
  return 0;
}

static const
luaL_reg LogInstanceMethods[] = {
  {"append", arken_LogInstanceMethodAppend},
  {"dump", arken_LogInstanceMethodDump},
  {"log", arken_LogInstanceMethodLog},
  {"trace", arken_LogInstanceMethodTrace},
  {"debug", arken_LogInstanceMethodDebug},
  {"info", arken_LogInstanceMethodInfo},
  {"warn", arken_LogInstanceMethodWarn},
  {"error", arken_LogInstanceMethodError},
  {"fatal", arken_LogInstanceMethodFatal},
  {"__gc", arken_LogInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerLogInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "Log.metatable");
  luaL_register(L, NULL, LogInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_Log( lua_State *L ) {
    registerLogInstanceMethods(L);
    registerLogClassMethods(L);
    return 1;
  }
}
