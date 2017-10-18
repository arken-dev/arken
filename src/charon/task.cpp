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

QThreadPool * task::s_pool = 0;
QList<task::worker *> * task::s_workers = new QList<task::worker *>;
QMutex task::s_mutex;

char * task::start(const char * fileName, const char * data)
{
  char * uuid = os::uuid();
  cache::insert(uuid, data);
  task::worker * worker = new task::worker(uuid, fileName, data);

  task::gc();

  worker->start();
  s_workers->append(worker);

  return uuid;
}

int task::gc()
{
  int result = 0;
  QMutexLocker ml(&s_mutex);
  for(int i = 0; i < s_workers->size(); i++) {
    task::worker * wkr = s_workers->at(i);
    if( wkr->isFinished() && wkr->finishedAt() > 30 ) {
      s_workers->removeOne(wkr);
      delete wkr;
      result++;
    }
  }
  return result;
}

int task::wait()
{
  int result = 0;
  QMutexLocker ml(&s_mutex);
  for(int i = 0; i < s_workers->size(); i++) {
    task::worker * wkr = s_workers->at(i);
    if( ! wkr->isFinished() ) {
      wkr->wait();
      result++;
    }
  }
  return result;
}

char * task::pool(const char * fileName, const char * data)
{
  char * uuid = os::uuid();
  cache::insert(uuid, data);
  if( s_pool == 0 ) {
    s_pool = new QThreadPool;
    s_pool->setMaxThreadCount(os::cores());
  }

  s_pool->start(new task::worker(uuid, fileName, data));

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

task::worker::worker(const char * uuid, const char * fileName, const char * data)
{
  m_microtime = 0;
  m_uuid     = uuid;
  m_fileName = fileName;
  m_data     = data;
}

double task::worker::finishedAt()
{
  if( m_microtime == 0 ) {
    return 0;
  } else {
    return os::microtime() - m_microtime;
  }
}

task::worker::~worker()
{
  qDebug() << "destructor service ..." << m_fileName;
  cache::remove(m_uuid.data());
}

void task::worker::run()
{
  int rv;
  charon::instance i = mvm::instance();
  lua_State * luaState = i.state();

  // CHARON_TASK
  lua_pushstring(luaState, m_uuid);
  lua_setglobal(luaState, "CHARON_TASK");

  //call
  rv = luaL_loadfile(luaState, m_fileName.data());
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(luaState, -1));
    return;
  }

  rv = lua_pcall(luaState, 0, 0, lua_gettop(luaState) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(luaState, -1));
    return;
  }

  // clear CHARON_TASK
  lua_pushboolean(luaState, false);
  lua_setglobal(luaState, "CHARON_TASK");

  // lua gc
  lua_gc(luaState, LUA_GCCOLLECT, 0);

  // microtime
  m_microtime = os::microtime();
}
