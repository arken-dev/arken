// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/cache>
#include <arken/task>
#include <arken/mvm>

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * params);

using namespace arken::concurrent;
using cache = arken::cache;

void task::wait()
{
  mvm::wait();
}

task::~task()
{ }

bool task::release()
{
  return m_release;
}

void task::run()
{
  arken::instance i = mvm::instance();
  lua_State * L = i.state();
  lua_settop(L, 0);

  int rv;

  lua_settop(L, 0);
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

  lua_pushlstring(L,  m_uuid, 37);
  json_lock_decode(L, m_params);

  rv = lua_pcall(L, 2, 0, 0); // alterar para envio de 1
  if (rv) {
    fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
  }

  // GC
  if( m_purge ) {
    i.release();
    lua_close(L);
  } else {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }

} // task::run

string task::start(const char * fileName, const char * params, bool purge)
{
  task * ptr = new task(fileName, params, purge);
  mvm::concurrent(ptr);
  return ptr->m_uuid;
}

task::task(const char * fileName, const char * params, bool purge)
{
  m_fileName = fileName;
  m_uuid     = os::uuid();
  m_params   = params;
  m_purge    = purge;
  m_release  = true;
}
