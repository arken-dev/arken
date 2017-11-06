// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <QDebug>
#include <lua/lua.hpp>
#include <charon/base>
#include <charon/cache>
#include <charon/mvm>
#include <charon/service>

using namespace charon;

using charon::cache;
using charon::mvm;

QList<service::worker *> * service::s_workers = new QList<service::worker *>;
QMutex service::s_mutex;

char * service::start(const char * fileName)
{
  char * uuid = os::uuid();
  cache::insert(uuid, "");
  service::worker * worker = new service::worker(uuid, fileName);

  service::gc();

  worker->start();
  s_workers->append(worker);

  return uuid;
}

int service::gc()
{
  int result = 0;
  QMutexLocker ml(&s_mutex);
  for(int i = 0; i < s_workers->size(); i++) {
    service::worker * wkr = s_workers->at(i);
    if( wkr->isFinished() && wkr->finishedAt() > 30 ) {
      s_workers->removeOne(wkr);
      delete wkr;
      result++;
    }
  }
  return result;
}

service::worker::worker(const char * uuid, const char * fileName)
{
  m_microtime = 0;
  m_uuid      = uuid;
  m_fileName  = fileName;
  m_version   = mvm::version();
}

double service::worker::finishedAt()
{
  if( m_microtime == 0 ) {
    return 0;
  } else {
    return os::microtime() - m_microtime;
  }
}

bool service::worker::loop(int secs)
{
  int i = 0;

  while( i < secs ) {

    QThread::msleep(1000);

    if(isShutdown()) {
      return false;
    }

    i++;
  }

  return true;
}

bool service::worker::isShutdown()
{
  if( m_version != mvm::version() ) {
    return true;
  }
  return false;
}

const char * service::worker::uuid()
{
  return m_uuid.data();
}

service::worker::~worker()
{
  qDebug() << "destructor service ..." << m_fileName;
  cache::remove(m_uuid.data());
}

void service::worker::run()
{
  int rv;
  charon::instance i = mvm::instance();
  lua_State * luaState = i.state();

  // CHARON_TASK
  lua_pushstring(luaState, m_uuid);
  lua_setglobal(luaState, "CHARON_SERVICE");

  //allocate
  lua_pushlightuserdata(luaState, this);
  lua_setglobal(luaState, "__charon_service");

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
  lua_setglobal(luaState, "CHARON_SERVICE");

  // clear this
  lua_pushnil(luaState);
  lua_setglobal(luaState, "__charon_service");

  // lua gc
  lua_gc(luaState, LUA_GCCOLLECT, 0);

  service::start(this->m_fileName);

  // microtime
  m_microtime = os::microtime();
}
