// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <iostream>
#include <fstream>
#include <arken/net/config.h>
#include <arken/log.h>
#include <arken/concurrent/service.h>
#include <lua/lua.hpp>

namespace arken {
namespace net {

using mvm     = arken::mvm;
using Config  = arken::net::Config;
using Log     = arken::Log;
using service = arken::concurrent::service;

Config::Config(string path)
{
  if ( ! os::exists("dispatch.lua") ) {
    std::cerr << "dispatch.lua not found" << std::endl;
    throw;
  }

  if( !os::exists(path) && !path.endsWith("server.json") ) {
    path = "config/server.json";
  }

  if( os::exists(path) ) {
    std::cout << "using " << path << std::endl;

    string raw = os::read(path);

    // arken instance
    arken::instance i = mvm::instance();
    lua_State * L = i.state();

    // Process Request
    // TODO return is not validate
    lua_settop(L, 0);
    lua_getglobal(L,  "require");
    lua_pushstring(L, "arken.json");
    int rv = lua_pcall(L, 1, 1, 0);
    if (rv) {
      fprintf(stderr, "primeiro erro %s\n", lua_tostring(L, -1));
      throw;
    }

    lua_pushstring(L, "decode");
    lua_gettable(L, -2);

    lua_pushlstring(L, raw.data(), raw.size());

    if( lua_pcall(L, 1, 1, 0) != 0 ) {
      fprintf(stderr, "segundo erro %s\n", lua_tostring(L, -1));
    }

    //---------------------------------------------------------------------------
    // ADDRESS
    //---------------------------------------------------------------------------

    lua_pushstring(L, "address");
    lua_gettable(L, -2);
    if( lua_isnil(L, -1) ) {
      std::cout << "warning address not found use 127.0.0.1" << std::endl;
      m_address = "127.0.0.1";
    } else {
      m_address = lua_tostring(L, -1);
    }
    lua_pop(L, 1);

    //---------------------------------------------------------------------------
    // LOGS
    //---------------------------------------------------------------------------

    lua_pushstring(L, "log");
    lua_gettable(L, -2);
    if( lua_isnil(L, -1) ) {
      std::cout << "log path not found use logs/server.log" << std::endl;
      m_log = "logs/server.log";
    } else {
      m_log = lua_tostring(L, -1);
    }
    lua_pop(L, 1);

    //---------------------------------------------------------------------------
    // PORT
    //---------------------------------------------------------------------------

    lua_pushstring(L, "port");
    lua_gettable(L, -2);
    if( lua_isnil(L, -1) ) {
      std::cout << "warning port not found use 2345" << std::endl;
      m_port  = 2345;
    } else {
      m_port = lua_tointeger(L, -1);
    }
    lua_pop(L, 1);

    //---------------------------------------------------------------------------
    // THREADS
    //---------------------------------------------------------------------------

    lua_pushstring(L, "threads");
    lua_gettable(L, -2);
    if( lua_isnil(L, -1) ) {
      m_threads = os::cores();
      std::cout << "warning threads not found use " << m_threads << std::endl;
    } else {
      m_threads = lua_tointeger(L, -1);
    }
    lua_pop(L, 1);

    //---------------------------------------------------------------------------
    // PID
    //---------------------------------------------------------------------------

    lua_pushstring(L, "pid");
    lua_gettable(L, -2);
    if( lua_isnil(L, -1) ) {
      m_pid = "tmp/pid/server.pid";
      std::cout << "warning pid not found use " << m_pid << std::endl;
    } else {
      m_pid = lua_tostring(L, -1);
    }
    lua_pop(L, 1);

    //---------------------------------------------------------------------------
    // SERVICE
    //---------------------------------------------------------------------------

    lua_pushstring(L, "service");
    lua_gettable(L, -2);
    if( lua_isnil(L, -1) ) {
      m_service = false;
      std::cout << "warning service not found use false" << std::endl;
    } else {
      m_service = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    //---------------------------------------------------------------------------
    // REMOVE TABLE
    //---------------------------------------------------------------------------

    lua_pop(L, 1);

  } else {
    std::cout << path << " not found using default values" << std::endl;
    m_threads = os::cores();
    m_port    = 2345;
    m_address = "127.0.0.1";
    m_service = false;
    m_pid     = "tmp/pid/server.pid";
    m_log     = "logs/server.log";
  }

  //---------------------------------------------------------------------------
  // SERVICE
  //---------------------------------------------------------------------------

  if( m_service && os::exists("app/services")) {
    arken::concurrent::service::load("app/services");
  }

  //---------------------------------------------------------------------------
  // PID
  //---------------------------------------------------------------------------

  std::cout << "write pid: " << m_pid << std::endl;
  std::ofstream pidfile;
  pidfile.open(m_pid);
  pidfile << os::pid();
  pidfile.close();

  //---------------------------------------------------------------------------
  // LOGS
  //---------------------------------------------------------------------------

  Log log = Log(m_log.c_str());
  log.info("iniciando server");
  if( m_service ) {
    if( os::exists("app/services") ) {
      log.info("services started");
    } else {
      log.info("services dir not exists");
    }
  } else {
    log.info("services not enable");
  }
  log.dump();

}

std::string Config::address()
{
  return m_address;
}

std::string Config::pid()
{
  return m_pid;
}

unsigned int Config::port()
{
  return m_port;
}

unsigned int Config::threads()
{
  return m_threads;
}

bool Config::service()
{
  return m_service;
}

} // namespace net
} // namespace arken
