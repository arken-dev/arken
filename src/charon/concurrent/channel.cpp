// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/cache>
#include <charon/mvm>

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * params);

using namespace charon::concurrent;

void channel::wait()
{
  /*
  while( true ) {
    {
      std::unique_lock<std::mutex> lck(*channel::mtx);
      if (channel::actives == 0 && queue->empty()) {
        return;
      }
    }
    // TODO improved whithout sleep
    os::sleep(0.05);
  }
  */
}

void channel::run()
{
  charon::instance i = mvm::instance();
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

  lua_getglobal(L, "require");
  lua_pushstring(L, m_fileName);
  rv = lua_pcall(L, 1, 1, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  //lua_pushlstring(L,  m_uuid, 37); //push channel
  channel **ptr = (channel **)lua_newuserdata(L, sizeof(channel*));
  *ptr = this;
  luaL_getmetatable(L, "charon.concurrent.channel.metatable");
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
  if( m_release ) {
    i.release();
    lua_close(L);
  } else {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }

} // channel::run

channel::channel(
  std::queue<std::string> * read,
  std::queue<std::string> * write,
  std::mutex * read_mtx,
  std::mutex * write_mtx,
  std::condition_variable * read_condition,
  std::condition_variable * write_condition
)
{
  m_read  = read;
  m_write = write;
  m_read_mtx = read_mtx;
  m_write_mtx = write_mtx;
  m_read_condition  = read_condition;
  m_write_condition = write_condition;
  m_finished = false;
  m_release  = false;
}

channel::channel(const char * fileName, const char * params, bool purge)
{
  m_read  = new std::queue<std::string>;
  m_write = new std::queue<std::string>;
  m_read_mtx  = new std::mutex;
  m_write_mtx = new std::mutex;
  m_read_condition  = new std::condition_variable;
  m_write_condition = new std::condition_variable;

  m_fileName = fileName;
  m_params   = params;
  m_purge    = purge;
  m_finished = false;
  m_release  = false;
}

channel * channel::start(const char * fileName, const char * params, bool purge)
{
  channel * c = new channel(fileName, params, purge);
  mvm::concurrent(c);
  channel * client = new channel(
    c->m_write, c->m_read, c->m_write_mtx, c->m_read_mtx, c->m_write_condition, c->m_read_condition
  );
  c->m_client = client;
  return client;
}

bool channel::release()
{
  return m_release;
}

channel::~channel()
{
  delete m_read;
  delete m_read_mtx;
  delete m_read_condition;
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
