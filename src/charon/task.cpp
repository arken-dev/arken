// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/cache>
#include <charon/task>
#include <charon/mvm>

using namespace charon;

using charon::cache;

std::vector<std::thread> * task::workers   = new std::vector<std::thread>;
std::queue<task::work *> * task::queue     = new std::queue<task::work *>;
std::mutex               * task::mtx       = new std::mutex;
std::condition_variable  * task::condition = new std::condition_variable;
uint32_t task::max = os::cores();
uint32_t task::actives = 0;

task::work * task::get()
{
  task::work * work = 0;
  std::unique_lock<std::mutex> lck(*task::mtx);
  condition->wait(lck, []{ return !queue->empty(); });
  work = queue->front();
  queue->pop();
  task::actives++;
  return work;
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

  while( true ) {
    task::work * work = task::get();

    charon::instance i = mvm::instance();
    lua_State * L = i.state();
    lua_settop(L, 0);

    // CHARON_TASK
    lua_pushstring(L, work->uuid());
    lua_setglobal(L, "CHARON_TASK");

    int rv;

    //TODO .lua end file use dofile not use require ???
    lua_getglobal(L, "dofile");
    lua_pushstring(L, work->fileName());
    delete work;

    rv = lua_pcall(L, 1, 0, 0);
    if (rv) {
      fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
    }

    // clear CHARON_TASK
    lua_pushboolean(L, false);
    lua_setglobal(L, "CHARON_TASK");
    // lua gc
    lua_gc(L, LUA_GCCOLLECT, 0);

    mtx->lock();
    task::actives--;
    mtx->unlock();

  } // while

} // task::work

char * task::start(const char * fileName, const char * data)
{
  char * uuid = os::uuid();
  cache::insert(uuid, data);
  std::unique_lock<std::mutex> lck(*task::mtx);

  if( workers->size() < task::max && (workers->size() - task::actives) == 0  ) {
      workers->push_back(std::thread(task::run));
  }
  queue->push(new task::work(uuid, fileName));
  condition->notify_one();

  return uuid;
}

const char * task::value(const char * uuid)
{
  return cache::value(uuid);
}

void task::insert(const char * uuid, const char * data)
{
  cache::insert(uuid, data);
}

void task::set(uint32_t max)
{
  task::max = max;
}

task::work::work(const char * uuid, const char * fileName)
{
  m_uuid     = uuid;
  m_fileName = fileName;
}

const char * task::work::uuid()
{
  return m_uuid.c_str();
}

const char * task::work::fileName()
{
  return m_fileName.c_str();
}
