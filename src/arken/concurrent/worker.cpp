// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/mvm.h>
#include <arken/concurrent/worker.h>
#include <vector>
#include <arken/json.h>

namespace arken {
namespace concurrent {

std::atomic<uint32_t> worker::s_max{0};

worker::~worker() = default;

void worker::run()
{
  int rv;
  mvm::instance instance = mvm::getInstance();
  instance.swap(m_shared);

  lua_State * L = instance.state();
  lua_settop(L, 0);
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.concurrent.worker");
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

  if(!lua_istable(L, -1)) {
    luaL_error(L, "table not found");
  }

  //---------------------------------------------------------------------------
  // VALIDATE START
  //---------------------------------------------------------------------------

  lua_pushstring(L, "start");
  lua_gettable(L, -2);
  if( lua_isfunction (L, -1) ) {
    lua_settop(L, -2);
  } else {
    std::cerr << "start function is requried" << std::endl;
    return;
  }


  //---------------------------------------------------------------------------
  // VALIDATE RUN
  //---------------------------------------------------------------------------

  lua_pushstring(L, "run");
  lua_gettable(L, -2);
  if( lua_isfunction (L, -1) ) {
    lua_settop(L, -2);
  } else {
    std::cerr << "run function is requried" << std::endl;
    return;
  }

  //---------------------------------------------------------------------------
  // START
  //---------------------------------------------------------------------------

  lua_pushstring(L, "start");
  lua_gettable(L, -2);

  auto ptr = static_cast<worker **>(lua_newuserdata(L, sizeof(worker *)));
  *ptr = this;
  luaL_getmetatable(L, "arken.concurrent.worker.metatable");
  lua_setmetatable(L, -2);

  json::decode(L, m_params);

  if( lua_pcall(L, 2, 0, 0) != 0 ) {
    fprintf(stderr, "start => %s\n", lua_tostring(L, -1));
  }

  if(!lua_istable(L, -1)) {
    luaL_error(L, "no final nao e table table not found");
  }

  //---------------------------------------------------------------------------
  // PERFORM
  //---------------------------------------------------------------------------

  unsigned int cores;

  if( s_max ) {
    cores = s_max;
  } else {
    cores = os::cores();
  }

  if( m_queue.size() < cores ) {
    cores = m_queue.size();
  }

  (*m_total.get()) = m_queue.size();

  this->perform(cores);

  //---------------------------------------------------------------------------
  // STOP
  //---------------------------------------------------------------------------

  lua_pushstring(L, "stop");
  lua_gettable(L, -2);
  if( lua_isfunction (L, -1) ) {
    auto stop = static_cast<worker **>(lua_newuserdata(L, sizeof(worker *)));
    *stop = this;
    luaL_getmetatable(L, "arken.concurrent.worker.metatable");
    lua_setmetatable(L, -2);

    json::decode(L, m_params);

    if( lua_pcall(L, 2, 0, 0) != 0 ) {
      fprintf(stderr, "stop => %s\n", lua_tostring(L, -1));
    }
    lua_settop(L, -2);
  }

} // worker::run

worker worker::start(const char * fileName, const char * params, bool release)
{
  auto ptr = new worker(fileName, params, release);
  arken::concurrent::core::start(ptr);
  return worker(*ptr);
}

worker::worker(const char * fileName, const char * params, bool release)
{
  m_uuid     = os::uuid();
  m_fileName = fileName;
  m_params   = params;
  m_release  = release;
  m_total    = std::shared_ptr<std::atomic<int>>(new std::atomic<int>(0));
  m_progress = std::shared_ptr<std::atomic<int>>(new std::atomic<int>(0));

  m_shared.name("arken.concurrent.worker");
}

worker::worker(const worker &obj)
{
  m_uuid     = obj.m_uuid;
  m_fileName = obj.m_fileName;
  m_params   = obj.m_params;
  m_release  = obj.m_release;
  m_finished = obj.m_finished;
  m_progress = obj.m_progress;
  m_total    = obj.m_total;
  m_shared   = obj.m_shared;
}

void worker::enqueue(string && value)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  m_queue.push(std::move(value));
}

void worker::increment()
{
  (*m_progress.get())++;
}

float worker::progress()
{
  if( *m_total.get() ) {
    return (*m_progress.get() * 100.0 ) / *m_total.get();
  } else {
    return 0;
  }
}

string worker::node::dequeue()
{
  std::unique_lock<std::mutex> lck(m_worker->m_mutex);
  if( m_worker->m_queue.empty() ) {
    return {};
  } else {
    string result = std::move(m_worker->m_queue.front());
    m_worker->m_queue.pop();
    return result;
  }
}

worker::node::node(worker * ptr, string fileName, uint32_t number, string params, bool release)
{
  m_uuid     = os::uuid();
  m_worker   = ptr;
  m_fileName = fileName;
  m_number   = number;
  m_params   = params;
  m_release  = release;
  m_shared   = m_worker->m_shared;
  m_shared.name("arken.concurrent.worker.node#");
  m_shared.name().append(std::to_string(m_number));
}

worker::node::node(const worker::node &obj)
{
  m_number    = obj.m_number;
  m_fileName  = obj.m_fileName;
  m_worker    = obj.m_worker;
  m_params    = obj.m_params;
  m_release   = obj.m_release;
  m_finished  = obj.m_finished;
  m_shared    = obj.m_shared;
  m_uuid      = obj.m_uuid;
  m_microtime = obj.m_microtime;
}


void worker::node::run()
{

  int rv;
  mvm::instance instance = mvm::getInstance();
  instance.swap(m_shared);

  lua_State * L = instance.state();
  lua_settop(L, 0);
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.concurrent.worker");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  lua_getglobal(L, "require");
  lua_pushstring(L, m_fileName);

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
      fprintf(stderr, "file => %s\n", lua_tostring(L, -1));
    }
  }

  //---------------------------------------------------------------------------
  // BEFORE
  //---------------------------------------------------------------------------

  lua_pushstring(L, "before");
  lua_gettable(L, -2);
  if( lua_isfunction (L, -1) ) {
    auto ptr = static_cast<worker::node **>(lua_newuserdata(L, sizeof(worker::node *)));
    *ptr = this;
    luaL_getmetatable(L, "arken.concurrent.worker.node.metatable");
    lua_setmetatable(L, -2);

    if( lua_pcall(L, 1, 0, 0) != 0 ) {
      fprintf(stderr, "before => %s: %s\n", m_fileName.data(), lua_tostring(L, 2));
    }
  } else {
    lua_settop(L, -2);
  }

  //---------------------------------------------------------------------------
  // RUN
  //---------------------------------------------------------------------------
  while( true ) {

    string value = dequeue();

    if( value.empty() ) {
      break;
    }

    lua_pushstring(L, "run");
    lua_gettable(L, -2);

    auto ptr = static_cast<worker::node **>(lua_newuserdata(L, sizeof(worker::node *)));
    *ptr = this;
    luaL_getmetatable(L, "arken.concurrent.worker.node.metatable");
    lua_setmetatable(L, -2);
    json::decode(L, value.data());

    if( lua_pcall(L, 2, 0, 0) != 0 ) {
      fprintf(stderr, "run => %s: %s\n", m_fileName.data(), lua_tostring(L, -1));
    }

    this->m_worker->increment();
  }

  //---------------------------------------------------------------------------
  // AFTER
  //---------------------------------------------------------------------------

  lua_pushstring(L, "after");
  lua_gettable(L, -2);
  if( lua_isfunction (L, -1) ) {
    auto ptr  = static_cast<worker::node **>(lua_newuserdata(L, sizeof(worker::node *)));
    *ptr = this;
    luaL_getmetatable(L, "arken.concurrent.worker.node.metatable");
    lua_setmetatable(L, -2);

    if( lua_pcall(L, 1, 0, 0) != 0 ) {
      fprintf(stderr, "after => %s: %s\n", m_fileName.data(), lua_tostring(L, 2));
    }
  } else {
    lua_settop(L, -2);
  }

  // GC
  if( m_release ) {
    instance.release();
  } else {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }

}

uint32_t worker::node::number()
{
  return m_number;
}

worker worker::node::master()
{
  return worker(*m_worker);
}

} // namespace concurrent
} // namespace arken
