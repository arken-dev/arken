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

std::vector<std::thread>   * task::workers = new std::vector<std::thread>;
std::queue<task::worker *> * task::queue   = new std::queue<task::worker *>;
std::mutex                 * task::mtx     = new std::mutex;

void task::run()
{

  while( true ) {
     task::worker * wkr = 0;

    mtx->lock();
    if(! queue->empty() ) {
      wkr = queue->front();
      queue->pop();
    }
    mtx->unlock();

    if( wkr ) {

      charon::instance i = mvm::instance();
      lua_State * L = i.state();
      lua_settop(L, 0);

      // CHARON_TASK
      lua_pushstring(L, wkr->uuid());
      lua_setglobal(L, "CHARON_TASK");

      int rv;

      //TODO .lua end file use dofile not use require ???
      lua_getglobal(L, "dofile");
      lua_pushstring(L, wkr->fileName());
      delete wkr;

      rv = lua_pcall(L, 1, 0, 0);
      if (rv) {
        fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
      }

      // clear CHARON_TASK
      lua_pushboolean(L, false);
      lua_setglobal(L, "CHARON_TASK");
      // lua gc
      lua_gc(L, LUA_GCCOLLECT, 0);
    } //if

    // TODO future ???
    os::sleep(0.10);

  } // while

} // task::worker

const char * task::start(const char * fileName, const char * data)
{
  char * uuid = os::uuid();
  size_t size = strlen(fileName);
  char * f = new char[size+37];
  strcpy(f, fileName);
  f[size] = '\0';

  cache::insert(uuid, data);

  mtx->lock();
  if( workers->size() == 0 ) {
    // TODO fix static 5 pool size
    for(int i = 0; i < 5; i++ ) {
      workers->push_back(std::thread(task::run));
    }
  }
  mtx->unlock();

  mtx->lock();
  queue->push(new task::worker(uuid, f));
  mtx->unlock();

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

task::worker::worker(char * uuid, char * fileName)
{
  m_uuid     = uuid;
  m_fileName = fileName;
}

task::worker::~worker()
{
  delete[] m_uuid;
  delete[] m_fileName;
}

char * task::worker::uuid()
{
  return m_uuid;
}

char * task::worker::fileName()
{
  return m_fileName;
}
