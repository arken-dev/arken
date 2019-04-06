// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/net/config.h>
#include <lua/lua.hpp>

using mvm = charon::mvm;

using namespace charon::net;

Config::Config(string path)
{
  string raw = os::read(path);

  // charon instance
  charon::instance i = mvm::instance();
  lua_State * L = i.state();

  // Process Request
  // TODO return is not validate
  lua_settop(L, 0);
  lua_getglobal(L,  "require");
  lua_pushstring(L, "charon.json");
  int rv = lua_pcall(L, 1, 1, 0);
  if (rv) {
    fprintf(stderr, "primeiro erro %s\n", lua_tostring(L, -1));
    throw;
  }

  lua_pushstring(L, "decode");
  lua_gettable(L, -2);

  lua_pushstring(L, raw);

  if( lua_pcall(L, 1, 1, 0) != 0 ) {
    fprintf(stderr, "segundo erro %s\n", lua_tostring(L, -1));
  }

  //---------------------------------------------------------------------------
  // ADDRESS
  //---------------------------------------------------------------------------

  lua_pushstring(L, "address");
  lua_gettable(L, -2);
  m_address = lua_tostring(L, -1);
  lua_pop(L, 1);

  //---------------------------------------------------------------------------
  // PORT
  //---------------------------------------------------------------------------

  lua_pushstring(L, "port");
  lua_gettable(L, -2);
  m_port = lua_tointeger(L, -1);
  lua_pop(L, 1);

  //---------------------------------------------------------------------------
  // THREADS
  //---------------------------------------------------------------------------

  lua_pushstring(L, "threads");
  lua_gettable(L, -2);
  m_threads = lua_tointeger(L, -1);
  lua_pop(L, 1);

  //---------------------------------------------------------------------------
  // PID
  //---------------------------------------------------------------------------

  lua_pushstring(L, "pid");
  lua_gettable(L, -2);
  m_pid = lua_tostring(L, -1);
  lua_pop(L, 1);

  //---------------------------------------------------------------------------
  // SERVICE
  //---------------------------------------------------------------------------

  lua_pushstring(L, "service");
  lua_gettable(L, -2);
  m_service = lua_toboolean(L, -1);
  lua_pop(L, 1);

  //---------------------------------------------------------------------------
  // REMOVE TABLE
  //---------------------------------------------------------------------------

  lua_pop(L, 1);
}

std::string Config::address()
{
  return m_address;
}

std::string Config::pid()
{
  return m_pid;
}

std::string Config::port()
{
  return m_port;
}

int Config::threads()
{
  return m_threads;
}

bool Config::service()
{
  return m_service;
}
