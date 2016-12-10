// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <CThread>

/**
 * checkCThread
 */

CThread *
checkCThread( lua_State *L ) {
  return *(CThread **) luaL_checkudata(L, 1, "CThread.metatable");
}

/**
 * ClassMethods
 */

static int
lua_CThreadClassMethodNew( lua_State *L ) {

  /* PATH */
  lua_getglobal(L, "CHARON_PATH");
  const char * CHARON_PATH = lua_tostring(L, -1);

  /* Thread */
  const char *str = (char *) lua_tostring(L, 1);
  CThread   **ptr = (CThread **)lua_newuserdata(L, sizeof(CThread*));
  *ptr= new CThread(CHARON_PATH, str);
  luaL_getmetatable(L, "CThread.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const luaL_reg CThreadClassMethods[] = {
  {"new", lua_CThreadClassMethodNew},
  {NULL, NULL}
};

void static
registerCThreadClassMethods( lua_State *L ) {
  luaL_register(L, "CThread", CThreadClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_CThreadInstanceMethodDestruct( lua_State *L ) {
  CThread *thread = checkCThread( L );
  delete thread;
  return 0;
}

static int
lua_CThreadInstanceMethodStart( lua_State *L ) {
  CThread *thread = checkCThread( L );
  thread->start();
  return 0;
}

static int
lua_CThreadInstanceMethodSetProperty( lua_State *L ) {
  bool result = false;
  bool flag   = true;

  CThread    * thread = checkCThread( L );
  const char * name   = luaL_checkstring(L, 2);

  if(lua_isboolean(L, 3)) {
    flag   = false;
    result = thread->setProperty(name, (bool) lua_toboolean(L, 3));
  }

  if(flag && lua_isnumber(L, 3)) {
    flag   = false;
    result = thread->setProperty(name, (double) lua_tonumber(L, 3));
  }

  if(flag && lua_isstring(L, 3)) {
    flag   = false;
    result = thread->setProperty(name, QByteArray(lua_tostring(L, 3)));
  }

  lua_pushboolean(L, result);

  return 1;
}

static int
lua_CThreadInstanceMethodWait( lua_State *L ) {
  CThread *thread = checkCThread( L );
  bool result  = thread->wait();
  lua_pushboolean(L, result);
  return 1;
}

static int
lua_CThreadInstanceMethodProperty( lua_State *L ) {

  bool flag = true;
  CThread *thread = checkCThread( L );
  const char *property = lua_tostring(L, 2);
  lua_getglobal(thread->m_luaState, property);

  if(lua_isboolean(thread->m_luaState, -1)) {
    flag = false;
    lua_pushboolean(L, lua_toboolean(thread->m_luaState, -1));
  }

  if(flag && lua_isnumber(thread->m_luaState, -1)) {
    flag = false;
    lua_pushnumber(L, lua_tonumber(thread->m_luaState, -1));
  }

  if(flag && lua_isstring(thread->m_luaState, -1)) {
    flag = false;
    lua_pushstring(L, lua_tostring(thread->m_luaState, -1));
  }

  if(flag) {
    lua_pushnil(L);
  }

  return 1;
}

static const
luaL_reg CThreadInstanceMethods[] = {
  {"wait", lua_CThreadInstanceMethodWait},
  {"setProperty", lua_CThreadInstanceMethodSetProperty},
  {"property", lua_CThreadInstanceMethodProperty},
  {"start", lua_CThreadInstanceMethodStart},
  {"__gc", lua_CThreadInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerCThreadInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "CThread.metatable");
  luaL_register(L, NULL, CThreadInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_CThread( lua_State *L ) {
    registerCThreadClassMethods(L);
    registerCThreadInstanceMethods(L);
    return 1;
  }
}
