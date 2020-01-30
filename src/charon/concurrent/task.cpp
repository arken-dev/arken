// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/cache>
#include <charon/task>
#include <charon/mvm>

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * params);

using namespace charon::concurrent;
using cache = charon::cache;

std::vector<std::thread> * task::workers   = new std::vector<std::thread>;
std::queue<task *>       * task::queue     = new std::queue<task *>;
std::mutex               * task::mtx       = new std::mutex;
std::condition_variable  * task::condition = new std::condition_variable;

uint32_t task::max = os::cores();
uint32_t task::actives = 0;

task * task::get()
{
  task * t = 0;
  std::unique_lock<std::mutex> lck(*task::mtx);
  condition->wait(lck, []{ return !queue->empty(); });
  t = queue->front();
  queue->pop();
  return t;
}

void task::wait()
{
  while( true ) {
    {
      std::unique_lock<std::mutex> lck(*task::mtx);
      if (task::actives == 0 && queue->empty()) {
        return;
      }
    }
    // TODO improved whithout sleep
    os::sleep(0.05);
  }
}

void task::run()
{
  if( m_function == nullptr ) {

    charon::instance i = mvm::instance();
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
    if( m_release ) {
      i.release();
      lua_close(L);
    } else {
      lua_gc(L, LUA_GCCOLLECT, 0);
    }
  } else {
    m_function(this->m_uuid);
  }

  std::unique_lock<std::mutex> lck(*task::mtx);
  task::actives--;

} // task::run

void task::working()
{

  while( true ) {
    task * t = task::get();
    t->run();
    delete t;
  } // while

} // task::working

string task::push(task * task)
{

  std::unique_lock<std::mutex> lck(*task::mtx);

  if( workers->size() < task::max && (workers->size() - task::actives) == 0  ) {
    workers->push_back(std::thread(task::working));
  }

  queue->push(task);
  task::actives++;
  condition->notify_one();

  return string(task->m_uuid);
}

string task::start(const char * fileName, const char * params, bool release)
{
  return task::push(new task(fileName, params, release));
}

string task::start(void (*func)( const char * uuid), const char * params, bool release)
{
  return task::push(new task(func, params, release));
}

void task::set(uint32_t max)
{
  task::max = max;
}

task::task(const char * fileName, const char * params, bool release)
{
  m_fileName = fileName;
  m_function = nullptr;
  m_uuid     = os::uuid();
  m_params   = params;
  m_release  = release;
}

task::task(void (*func)(const char * uuid), const char * params, bool release)
{
  m_function = func;
  m_uuid     = os::uuid();
  m_params   = params;
  m_release  = release;
}

task::~task()
{

}
