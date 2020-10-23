// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/cache>
#include <arken/mvm>
#include <memory>

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * params);

using namespace arken::concurrent;

void channel::wait()
{
  mvm::wait();
}

void channel::run()
{
  arken::instance i = mvm::instance();
  lua_State * L = i.state();
  lua_settop(L, 0);

  int rv;

  lua_settop(L, 0);

  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.concurrent.channel");
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

  //lua_pushlstring(L,  m_uuid, 37); //push channel
  channel **ptr = (channel **)lua_newuserdata(L, sizeof(channel*));
  *ptr = this;
  luaL_getmetatable(L, "arken.concurrent.channel.metatable");
  lua_setmetatable(L, -2);

  json_lock_decode(L, m_params);

  rv = lua_pcall(L, 2, 0, 0); // alterar para envio de 1
  if (rv) {
    fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
  }

  m_write->push("channel is finished");
  m_write_condition->notify_one();
  m_client->m_finished = true;

  // GC
  if( m_purge ) {
    i.release();
    lua_close(L);
  } else {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }

} // channel::run

channel::channel(
  std::shared_ptr<std::queue<std::string>> read,
  std::shared_ptr<std::queue<std::string>> write,
  std::shared_ptr<std::mutex> read_mtx,
  std::shared_ptr<std::mutex> write_mtx,
  std::shared_ptr<std::condition_variable> read_condition,
  std::shared_ptr<std::condition_variable> write_condition
)
{
  m_read            = read;
  m_write           = write;
  m_read_mtx        = read_mtx;
  m_write_mtx       = write_mtx;
  m_read_condition  = read_condition;
  m_write_condition = write_condition;
  m_finished        = false;
  m_release         = true;
  m_client          = nullptr;
}

channel::channel(const char * fileName, const char * params, bool purge)
{
  m_read            = std::shared_ptr<std::queue<std::string>>(new std::queue<std::string>);
  m_write           = std::shared_ptr<std::queue<std::string>>(new std::queue<std::string>);
  m_read_mtx        = std::shared_ptr<std::mutex>(new std::mutex);
  m_write_mtx       = std::shared_ptr<std::mutex>(new std::mutex);
  m_read_condition  = std::shared_ptr<std::condition_variable>(new std::condition_variable);
  m_write_condition = std::shared_ptr<std::condition_variable>(new std::condition_variable);

  m_fileName = fileName;
  m_params   = params;
  m_purge    = purge;
  m_finished = false;
  m_release  = true;

  m_client = new channel(
    m_write, m_read, m_write_mtx, m_read_mtx, m_write_condition, m_read_condition
  );

}

channel * channel::client()
{
  return m_client;
}

channel * channel::start(const char * fileName, const char * params, bool purge)
{
  channel * c = new channel(fileName, params, purge);
  mvm::concurrent(c);

  return c->client();
}

bool channel::release()
{
  return m_release;
}

channel::~channel()
{
  //delete m_read;
  //delete m_read_mtx;
  //delete m_read_condition;
}

bool channel::empty()
{
  std::unique_lock<std::mutex> lck(*m_read_mtx);
  return m_read->empty();
}

void channel::write(std::string message)
{
  if( m_finished == false ) {
    std::unique_lock<std::mutex> lck(*m_write_mtx);
    m_write->push(message);
    m_write_condition->notify_one();
  }
}

std::string channel::read()
{
  std::unique_lock<std::mutex> lck(*m_read_mtx);

  if( m_finished == true && m_read->empty() ) {
    return std::string("channel is finished");
  }

  m_read_condition->wait(lck, [&]{ return !m_read->empty(); });

  std::string message = m_read->front();
  m_read->pop();

  return message;
}
