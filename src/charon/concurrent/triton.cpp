// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/mvm>
#include <charon/concurrent/triton.h>
#include <vector>

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * params);

using namespace charon::concurrent;

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
  charon::instance i = mvm::instance();
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

  triton **ptr = (triton **)lua_newuserdata(L, sizeof(triton *));
  *ptr = this;
  luaL_getmetatable(L, "charon.concurrent.triton.metatable");
  lua_setmetatable(L, -2);

  json_lock_decode(L, m_params);

  if( lua_pcall(L, 2, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
  }

  if(!lua_istable(L, -1)) {
    luaL_error(L, "no final nao e table table not found");
  }


  // --------------------------------------------------------------------------
  // PERFORM
  // --------------------------------------------------------------------------

  this->perform();

  // --------------------------------------------------------------------------
  // STOP
  // --------------------------------------------------------------------------

  lua_pushstring(L, "stop");
  lua_gettable(L, -2);

  triton **stop = (triton **)lua_newuserdata(L, sizeof(triton *));
  *stop = this;
  luaL_getmetatable(L, "charon.concurrent.triton.metatable");
  lua_setmetatable(L, -2);

  json_lock_decode(L, m_params);

  if( lua_pcall(L, 2, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
  }

} // triton::run

void triton::start(const char * fileName, const char * params, bool purge)
{
  triton * ptr = new triton(fileName, params, purge);
  mvm::concurrent(ptr);
}

triton::triton(const char * fileName, const char * params, bool purge)
{
  m_fileName = fileName;
  m_params   = params;
  m_purge    = purge;
  m_release  = true;
}

void triton::perform()
{

  std::vector<triton::node *> list;

  unsigned int cores = os::cores() - 1;
  for(unsigned int i=0; i < cores; i++) {
    triton::node *n = new triton::node(this, m_fileName, i);
    mvm::concurrent(n);
    list.push_back(n);
  }

  while( true ) {
    bool flag = true;
    for(triton::node *n : list) {
      if( n->finished() == false ) {
        flag = false;
      }

    }
    if( flag ) {
      return;
    }
    os::sleep(0.10);
  }

}

void triton::enqueue(string value)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  m_queue.push(value.data());
}

void triton::append(string key, string result)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  if( m_result.count(key.data()) > 0 ) {
    std::string * value = m_result.at(key.data());
    value->append(result.data());
  } else {
    m_result[key.data()] = new std::string(result.data());
  }
}

void triton::count(string key)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  if( m_total.count(key.data()) > 0 ) {
    int value = m_total.at(key.data());
    value++;
    m_total[key.data()] = value;
  } else {
    m_total[key.data()] = 1;
  }
}

int triton::total(string key)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  if( m_total.count(key.data()) > 0 ) {
    return m_total.at(key.data());
  } else {
    return 0;
  }
}

string triton::result(string key)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  if( m_result.count(key.data()) > 0 ) {
    return m_result.at(key.data())->c_str();
  } else {
    return "";
  }
}

const char * triton::node::dequeue()
{
  std::unique_lock<std::mutex> lck(m_triton->m_mutex);
  if( m_triton->m_queue.empty() ) {
    return nullptr;
  } else {
    const char * result = m_triton->m_queue.front().c_str();
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
  charon::instance i = mvm::instance();
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

  rv = lua_pcall(L, 1, 1, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
    throw;
  }

  while( true ) {

    const char * value = dequeue();
    if( value == nullptr ) {
      break;
    }

    lua_pushstring(L, "run");
    lua_gettable(L, -2);

    triton **ptr = (triton **)lua_newuserdata(L, sizeof(triton *));
    *ptr = this->m_triton;
    luaL_getmetatable(L, "charon.concurrent.triton.metatable");
    lua_setmetatable(L, -2);
    lua_pushstring(L, value);

    if( lua_pcall(L, 2, 0, 0) != 0 ) {
      fprintf(stderr, "file name => %s: %s\n", m_fileName.data(), lua_tostring(L, 2));
    }

  }

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
