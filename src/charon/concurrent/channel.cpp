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

std::vector<std::thread> * channel::workers   = new std::vector<std::thread>;
std::queue<channel *>    * channel::queue     = new std::queue<channel *>;
std::mutex               * channel::mtx       = new std::mutex;
std::condition_variable  * channel::condition = new std::condition_variable;

uint32_t channel::max = os::cores();
uint32_t channel::actives = 0;

channel * channel::get()
{
  channel * t = 0;
  std::unique_lock<std::mutex> lck(*channel::mtx);
  condition->wait(lck, []{ return !queue->empty(); });
  t = queue->front();
  queue->pop();
  return t;
}

void channel::wait()
{
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
}

void channel::run()
{
  if( m_function == nullptr ) {

    charon::instance i = mvm::instance();
    lua_State * L = i.state();
    lua_settop(L, 0);

    int rv;

    lua_settop(L, 0);

    lua_getglobal(L, "require");
    lua_pushstring(L, "charon.concurrent.channel");
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
  } else {
    //m_function(this->m_uuid); execute m_function with m_channel
  }

  std::unique_lock<std::mutex> lck(*channel::mtx);
  channel::actives--;

} // channel::run

void channel::working()
{

  while( true ) {
    channel * t = channel::get();
    t->run();
    delete t;
  } // while

} // channel::working

channel * channel::push(channel * c)
{

  std::unique_lock<std::mutex> lck(*channel::mtx);

  if( workers->size() < channel::max && (workers->size() - channel::actives) == 0  ) {
    workers->push_back(std::thread(channel::working));
  }

  queue->push(c);
  channel::actives++;
  condition->notify_one();

  channel * client = new channel(
    c->m_write, c->m_read, c->m_write_mtx, c->m_read_mtx, c->m_write_condition, c->m_read_condition
  );
  c->m_client = client;
  return client;
}

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
  m_function = nullptr;
  m_finished = false;
  m_release   = false;
}

channel::channel(const char * fileName, const char * params, bool release)
{
  m_read  = new std::queue<std::string>;
  m_write = new std::queue<std::string>;
  m_read_mtx  = new std::mutex;
  m_write_mtx = new std::mutex;
  m_read_condition  = new std::condition_variable;
  m_write_condition = new std::condition_variable;

  m_fileName = fileName;
  m_params   = params;
  m_function = nullptr;
  m_finished = false;
  m_release  = release;
}

channel::channel(void (*func)( channel * channel ), const char * params, bool release)
{
  m_read  = new std::queue<std::string>;
  m_write = new std::queue<std::string>;
  m_read_mtx  = new std::mutex;
  m_write_mtx = new std::mutex;
  m_read_condition  = new std::condition_variable;
  m_write_condition = new std::condition_variable;

  //m_fileName = nullptr;
  m_function = func;
  m_params   = params;
  m_finished = false;
  m_release  = release;
}

channel * channel::start(const char * fileName, const char * params, bool release)
{
  return channel::push(new channel(fileName, params, release));
}

channel * channel::start(void (*func)( channel * channel), const char * params, bool release)
{
  return channel::push(new channel(func, params, release));
}

void channel::set(uint32_t max)
{
  channel::max = max;
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
