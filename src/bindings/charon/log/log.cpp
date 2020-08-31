// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/log>

using charon::Log;

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
charon_LogClassMethodNew( lua_State *L ) {
  const char *str = (char *) luaL_checkstring(L, 1);
  Log **ptr = (Log **)lua_newuserdata(L, sizeof(Log*));
  *ptr = new Log(Log(str));
  luaL_getmetatable(L, "Log.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg LogClassMethods[] = {
  {"new", charon_LogClassMethodNew},
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
charon_LogInstanceMethodDestruct( lua_State *L ) {
  Log *udata = checkLog( L );
  delete udata;
  return 0;
}

static int
charon_LogInstanceMethodAppend( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->append(append);
  return 0;
}

static int
charon_LogInstanceMethodDump( lua_State *L ) {
  Log *udata  = checkLog( L );
  udata->dump();
  return 0;
}

static int
charon_LogInstanceMethodLog( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * level  = lua_tostring(L, 2);
  const char * append = lua_tostring(L, 3);
  udata->log(level, append);
  return 0;
}

static int
charon_LogInstanceMethodTrace( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->trace(append);
  return 0;
}

static int
charon_LogInstanceMethodDebug( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->debug(append);
  return 0;
}

static int
charon_LogInstanceMethodInfo( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->info(append);
  return 0;
}

static int
charon_LogInstanceMethodWarn( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->warn(append);
  return 0;
}

static int
charon_LogInstanceMethodError( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->error(append);
  return 0;
}

static int
charon_LogInstanceMethodFatal( lua_State *L ) {
  Log * udata  = checkLog( L );
  const char * append = lua_tostring(L, 2);
  udata->fatal(append);
  return 0;
}

static const
luaL_reg LogInstanceMethods[] = {
  {"append", charon_LogInstanceMethodAppend},
  {"dump", charon_LogInstanceMethodDump},
  {"log", charon_LogInstanceMethodLog},
  {"trace", charon_LogInstanceMethodTrace},
  {"debug", charon_LogInstanceMethodDebug},
  {"info", charon_LogInstanceMethodInfo},
  {"warn", charon_LogInstanceMethodWarn},
  {"error", charon_LogInstanceMethodError},
  {"fatal", charon_LogInstanceMethodFatal},
  {"__gc", charon_LogInstanceMethodDestruct},
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
  int
  luaopen_charon_Log( lua_State *L ) {
    registerLogInstanceMethods(L);
    registerLogClassMethods(L);
    return 1;
  }
}
