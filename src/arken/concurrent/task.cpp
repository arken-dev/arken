// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/os.h>
#include <arken/json.h>
#include <arken/concurrent/task.h>
#include <arken/concurrent/core.h>

namespace arken {
namespace concurrent {

task::~task()
{ }

void task::run()
{
  int rv;
  mvm::instance instance = mvm::getInstance( m_release );
  instance.swap(m_shared);

  lua_State * L = instance.state();

  lua_settop(L, 0);
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.concurrent.task");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  if( m_fileName.endsWith(".lua") ) {
    lua_getglobal(L, "dofile");
    lua_pushstring(L, m_fileName);
    rv = lua_pcall(L, 1, 1, 0);
    if (rv) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
    }
  } else {
    lua_getglobal(L, "require");
    lua_pushstring(L, m_fileName);
    rv = lua_pcall(L, 1, 1, 0);
    if (rv) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
    }
  }

  //lua_pushlstring(L,  m_uuid, 37);
  auto ptr = static_cast<task **>(lua_newuserdata(L, sizeof(task *)));
  *ptr = new task(*this);
  luaL_getmetatable(L, "arken.concurrent.task.metatable");
  lua_setmetatable(L, -2);

  json::decode(L, m_params);

  rv = lua_pcall(L, 2, 0, 0); // alterar para envio de 1
  if (rv) {
    fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
  }

  // GC
  if( m_release ) {
    instance.release();
  } else {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }

} // task::run

task task::start(const char * fileName, const char * params, bool release)
{
  auto ptr = new task(fileName, params, release);
  core::start(ptr);
  return task(*ptr);
}

task::task(const char * fileName, const char * params, bool release)
{
  m_fileName  = fileName;
  m_params    = params;
  m_release     = release;
  m_microtime = os::microtime();
  m_uuid      = os::uuid();

  m_shared.name("arken.concurrent.task");
}

task::task(const task &obj)
{
  m_fileName  = obj.m_fileName;
  m_params    = obj.m_params;
  m_release   = obj.m_release;
  m_finished  = obj.m_finished;
  m_shared    = obj.m_shared;
  m_uuid      = obj.m_uuid;
  m_microtime = obj.m_microtime;
}

} // namespace concurrent
} // namespace arken
