// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/helper>
#include <QtCore>

using charon::Thread;

/**
 * checkThread
 */

Thread *
checkThread( lua_State *L ) {
  return *(Thread **) luaL_checkudata(L, 1, "Thread.metatable");
}

/**
 * ClassMethods
 */

static int
lua_ThreadClassMethodNew( lua_State *L ) {

  /* PATH */
  lua_getglobal(L, "CHARON_PATH");
  const char * CHARON_PATH = lua_tostring(L, -1);

  /* Thread */
  const char *str = (char *) lua_tostring(L, 1);
  Thread   **ptr = (Thread **)lua_newuserdata(L, sizeof(Thread*));
  *ptr= new Thread(CHARON_PATH, str);
  luaL_getmetatable(L, "Thread.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const luaL_reg ThreadClassMethods[] = {
  {"new", lua_ThreadClassMethodNew},
  {NULL, NULL}
};

void static
registerThreadClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "Thread");
  luaL_register(L, NULL, ThreadClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
lua_ThreadInstanceMethodDestruct( lua_State *L ) {
  Thread *thread = checkThread( L );
  delete thread;
  return 0;
}

static int
lua_ThreadInstanceMethodStart( lua_State *L ) {
  Thread *thread = checkThread( L );
  thread->start();
  return 0;
}

static int
lua_ThreadInstanceMethodSetProperty( lua_State *L ) {
  bool result = false;
  bool flag   = true;

  Thread    * thread = checkThread( L );
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
lua_ThreadInstanceMethodWait( lua_State *L ) {
  Thread *thread = checkThread( L );
  bool result  = thread->wait();
  lua_pushboolean(L, result);
  return 1;
}

static int
lua_ThreadInstanceMethodProperty( lua_State *L ) {

  bool flag = true;
  Thread *thread = checkThread( L );
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
luaL_reg ThreadInstanceMethods[] = {
  {"wait", lua_ThreadInstanceMethodWait},
  {"setProperty", lua_ThreadInstanceMethodSetProperty},
  {"property", lua_ThreadInstanceMethodProperty},
  {"start", lua_ThreadInstanceMethodStart},
  {"__gc", lua_ThreadInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerThreadInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "Thread.metatable");
  luaL_register(L, NULL, ThreadInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_charon_Thread( lua_State *L ) {
    registerThreadInstanceMethods(L);
    registerThreadClassMethods(L);
    return 1;
  }
}
