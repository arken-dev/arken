// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/base>
#include <arken/mvm>
#include <arken/concurrent/triton.h>
#include <vector>

namespace arken {
namespace concurrent {

void triton::wait()
{
  mvm::wait();
}

triton::~triton()
{ }

bool triton::release()
{
  return m_release;
}

void triton::run()
{
  int rv;
  arken::instance i = mvm::instance();
  lua_State * L = i.state();
  lua_settop(L, 0);

  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.concurrent.triton");
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

  // --------------------------------------------------------------------------
  // START
  // --------------------------------------------------------------------------

  lua_pushstring(L, "start");
  lua_gettable(L, -2);

  auto ptr = static_cast<triton **>(lua_newuserdata(L, sizeof(triton *)));
  *ptr = this;
  luaL_getmetatable(L, "arken.concurrent.triton.metatable");
  lua_setmetatable(L, -2);

  json_lock_decode(L, m_params);

  if( lua_pcall(L, 2, 0, 0) != 0 ) {
    fprintf(stderr, "start => %s\n", lua_tostring(L, -1));
  }

  if(!lua_istable(L, -1)) {
    luaL_error(L, "no final nao e table table not found");
  }


  // --------------------------------------------------------------------------
  // PERFORM
  // --------------------------------------------------------------------------

  unsigned int cores = os::cores();
  if( m_queue.size() < cores ) {
    cores = m_queue.size();
  }

  this->perform(cores);

  // --------------------------------------------------------------------------
  // STOP
  // --------------------------------------------------------------------------

  lua_pushstring(L, "stop");
  lua_gettable(L, -2);

  auto stop = static_cast<triton **>(lua_newuserdata(L, sizeof(triton *)));
  *stop = this;
  luaL_getmetatable(L, "arken.concurrent.triton.metatable");
  lua_setmetatable(L, -2);

  json_lock_decode(L, m_params);

  if( lua_pcall(L, 2, 0, 0) != 0 ) {
    fprintf(stderr, "stop => %s\n", lua_tostring(L, -1));
  }

} // triton::run

string triton::start(const char * fileName, const char * params, bool purge)
{
  auto ptr = new triton(fileName, params, purge);
  mvm::concurrent(ptr);
  return ptr->m_uuid;
}

triton::triton(const char * fileName, const char * params, bool purge)
{
  m_uuid     = os::uuid();
  m_fileName = fileName;
  m_params   = params;
  m_purge    = purge;
  m_release  = true;
}

void triton::working(node * n)
{
  n->run();
  delete n;
}

void triton::perform(unsigned int cores)
{

  std::vector<std::thread *> threads;

  for(unsigned int i=0; i < cores; i++) {
    node *n = new node(this, m_fileName, i);
    threads.push_back(new std::thread(working, n));
  }

  for(std::thread *t : threads) {
    t->join();
  }

  for(std::thread *t : threads) {
    delete t;
  }

}

void triton::enqueue(string && value)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  m_queue.push(std::move(value));
}

void triton::append(string key, string result)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  if( m_result.count(key) > 0 ) {
    string * value = m_result.at(key);
    value->append(result);
  } else {
    m_result[key] = new string(result);
  }
}

void triton::count(string key)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  if( m_total.count(key) > 0 ) {
    int value = m_total.at(key);
    value++;
    m_total[key] = value;
  } else {
    m_total[key] = 1;
  }
}

int triton::total(string key)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  if( m_total.count(key) > 0 ) {
    return m_total.at(key);
  } else {
    return 0;
  }
}

string triton::uuid()
{
  return m_uuid;
}

string triton::result(string key)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  if( m_result.count(key) > 0 ) {
    return *m_result.at(key);
  } else {
    return "";
  }
}

string triton::node::dequeue()
{
  std::unique_lock<std::mutex> lck(m_triton->m_mutex);
  if( m_triton->m_queue.empty() ) {
    return {};
  } else {
    string result = std::move(m_triton->m_queue.front());
    m_triton->m_queue.pop();
    return result;
  }
}

triton::node::node(triton * ptr, string fileName, int number)
{

  m_triton   = ptr;
  m_fileName = fileName;
  m_number   = number;
}

void triton::node::run()
{

  int rv;
  arken::instance i = mvm::instance();
  lua_State * L = i.state();
  lua_settop(L, 0);

  lua_pushinteger(L, this->number());
  lua_setglobal(L, "TRITON_NUMBER");

  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.concurrent.triton");
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

  // --------------------------------------------------------------------------
  // BEFORE
  // --------------------------------------------------------------------------

  lua_pushstring(L, "before");
  lua_gettable(L, -2);

  auto ptr = static_cast<triton::node **>(lua_newuserdata(L, sizeof(triton::node *)));
  *ptr = this;
  luaL_getmetatable(L, "arken.concurrent.triton.node.metatable");
  lua_setmetatable(L, -2);

  if( lua_pcall(L, 1, 0, 0) != 0 ) {
    fprintf(stderr, "before => %s: %s\n", m_fileName.data(), lua_tostring(L, 2));
  }

  // --------------------------------------------------------------------------
  // RUN
  // --------------------------------------------------------------------------

  while( true ) {

    string value = dequeue();
    if( value.empty() ) {
      break;
    }

    lua_pushstring(L, "run");
    lua_gettable(L, -2);

    auto ptr = static_cast<triton::node **>(lua_newuserdata(L, sizeof(triton::node *)));
    *ptr = this;
    luaL_getmetatable(L, "arken.concurrent.triton.node.metatable");
    lua_setmetatable(L, -2);
    lua_pushstring(L, value);

    if( lua_pcall(L, 2, 0, 0) != 0 ) {
      fprintf(stderr, "run => %s: %s\n", m_fileName.data(), lua_tostring(L, 2));
    }

  }

  // --------------------------------------------------------------------------
  // AFTER
  // --------------------------------------------------------------------------

  lua_pushstring(L, "after");
  lua_gettable(L, -2);

  ptr  = static_cast<triton::node **>(lua_newuserdata(L, sizeof(triton::node *)));
  *ptr = this;
  luaL_getmetatable(L, "arken.concurrent.triton.node.metatable");
  lua_setmetatable(L, -2);

  if( lua_pcall(L, 1, 0, 0) != 0 ) {
    fprintf(stderr, "after => %s: %s\n", m_fileName.data(), lua_tostring(L, 2));
  }

  // TODO remove
  lua_pushnil(L);
  lua_setglobal(L, "TRITON_NUMBER");
}

bool triton::node::release()
{
  return false;
}

unsigned int triton::node::number()
{
  return m_number;
}

void triton::node::append(string key, string result)
{
  std::unique_lock<std::mutex> lck(m_triton->m_mutex);
  if( m_triton->m_result.count(key) > 0 ) {
    string * value = m_triton->m_result.at(key);
    value->append(result);
  } else {
    m_triton->m_result[key] = new string(result);
  }
}

void triton::node::count(string key)
{
  std::unique_lock<std::mutex> lck(m_triton->m_mutex);
  if( m_triton->m_total.count(key) > 0 ) {
    int value = m_triton->m_total.at(key);
    value++;
    m_triton->m_total[key] = value;
  } else {
    m_triton->m_total[key] = 1;
  }
}

int triton::node::total(string key)
{
  std::unique_lock<std::mutex> lck(m_triton->m_mutex);
  if( m_triton->m_total.count(key) > 0 ) {
    return m_triton->m_total.at(key);
  } else {
    return 0;
  }
}

string triton::node::result(string key)
{
  std::unique_lock<std::mutex> lck(m_triton->m_mutex);
  if( m_triton->m_result.count(key) > 0 ) {
    return *m_triton->m_result.at(key);
  } else {
    return "";
  }
}

string triton::node::uuid()
{
  return m_triton->m_uuid;
}

} // namespace concurrent
} // namespace arken
