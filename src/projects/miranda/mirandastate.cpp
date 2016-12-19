// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "mirandastate.h"
#include "mirandacache.h"
#include <QDebug>
#include <QStack>
#include <QReadWriteLock>

using charon::ByteArray;

int        MirandaState::s_gc          = 0;
qint64     MirandaState::s_version     = 0;
ByteArray  MirandaState::s_charonPath  = "";
ByteArray  MirandaState::s_profilePath = "";
QMutex     MirandaState::s_mutex;

QStack<MirandaState *> * MirandaState::s_stack   = new QStack<MirandaState *>;
QList<MirandaService*> * MirandaState::s_service = new QList<MirandaService *>;
QHash<ByteArray, MirandaCache *> * MirandaState::s_cache = new QHash<ByteArray, MirandaCache *>;
QThreadPool * MirandaState::s_pool = 0;
QReadWriteLock lock;


void miranda_cache_register(lua_State * L);
void miranda_server_register(lua_State * L);
void miranda_request_register(lua_State * L);
void miranda_service_register(lua_State * L);
void miranda_task_register(lua_State * L);

MirandaState::MirandaState()
{
  int rv;
  m_version = s_version;
  m_gc      = s_gc;
  m_State = luaL_newstate();

  luaL_openlibs(m_State);

  miranda_cache_register(m_State);
  miranda_server_register(m_State);
  miranda_request_register(m_State);
  miranda_service_register(m_State);
  miranda_task_register(m_State);

  if( strcmp(os::name(), "windows") == 0 ) {
    s_charonPath = s_charonPath.capitalize();
  }

  lua_pushstring(m_State, s_charonPath);
  lua_setglobal(m_State, "CHARON_PATH");

  rv = luaL_loadfile(m_State, s_profilePath);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  rv = lua_pcall(m_State, 0, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  luaL_loadfile(m_State, "dispatch.lua" );
  lua_pcall(m_State, 0, 0, 0);
  qDebug() << "create Lua State";
}

MirandaState::~MirandaState()
{
  lua_close(m_State);
}

void MirandaState::init(QCoreApplication *app)
{
  s_charonPath = app->applicationDirPath().toLocal8Bit();
  s_charonPath.truncate( s_charonPath.lastIndexOf('/') );

  s_profilePath = s_charonPath;
  s_profilePath.append("/profile.lua");

  servicesLoad();
}

void MirandaState::createService(QByteArray fileName)
{
  servicesAppend(new MirandaService(fileName));
}

void MirandaState::createTask(QByteArray fileName, const char * uuid)
{
  servicesAppend(new MirandaService(fileName, uuid));
}

void MirandaState::taskPool(QByteArray fileName, const char * uuid)
{
  if( s_pool == 0 ) {
    s_pool = new QThreadPool;
    s_pool->setMaxThreadCount(2);
  }

  s_pool->start(new MirandaService(fileName, uuid));
}

void MirandaState::servicesAppend(MirandaService *t)
{
  t->start(); // start thread

  QMutexLocker ml(&s_mutex);

  double time = os::microtime();
  double result;

  for(int i = 0; i < s_service->size(); i++) {
    MirandaService * service = s_service->at(i);
    result = time - service->time();
    //qDebug() << "result " << result << "\n\n\n\n\n";
    if( service->isFinished() and result > 30 ) {
      //qDebug() << "excluindo  " << " result is " << result;
      s_service->removeOne(service);
      delete service;
    }
  }

  s_service->append(t);

}

void MirandaState::servicesLoad()
{
  QString dir("app/services");
  if( QFile::exists(dir) ) {
    QDirIterator iterator(dir);
    while(iterator.hasNext()) {
      iterator.next();
      QFileInfo fileInfo = iterator.fileInfo();
      if( fileInfo.suffix() == "lua" ) {
        qDebug() << "load: " << fileInfo.filePath();
        MirandaState::createService(fileInfo.filePath().toLocal8Bit());
      }
    }
  } else {
    qDebug() << "services dir not exists";
  }
}

int MirandaState::gc()
{
  int i = 0;
  MirandaState * state;

  s_gc ++;
  state = takeFirst();

  while( state->m_gc != s_gc ) {
    lua_gc(state->instance(), LUA_GCCOLLECT, 0);
    state->m_gc = s_gc;
    push(state);
    i++;

    state = takeFirst();
  }

  push(state);

  return i;
}

MirandaState * MirandaState::pop()
{
  QMutexLocker ml(&s_mutex);
  MirandaState * state;

  if( s_stack->isEmpty() ) {
    state = new MirandaState();
  } else {
    state = s_stack->pop();
    if( s_version != state->m_version ) {
      delete state;
      state = new MirandaState();
    }
  }

  return state;
}

MirandaState * MirandaState::takeFirst()
{
  QMutexLocker ml(&s_mutex);

  if( s_stack->isEmpty() ) {
    return new MirandaState();
  }

  return s_stack->takeFirst();
}


void MirandaState::push(MirandaState * state)
{
  QMutexLocker ml(&s_mutex);
  s_stack->push(state);
}

void MirandaState::reload()
{
  s_version++;
  servicesReload();
}

// TODO check if new service is created
// before new reload
void MirandaState::servicesReload()
{
  int i;

  QMutexLocker ml(&s_mutex);

  for(i = 0; i < s_service->size(); i++) {
    MirandaService * service = s_service->at(i);
    service->shutdown();
  }

}

void MirandaState::clear()
{
  QMutexLocker ml(&s_mutex);
  s_version++;
  while( !s_stack->isEmpty() ) {
    delete s_stack->pop();
  }
}

lua_State * MirandaState::instance()
{
  return m_State;
}

int MirandaState::version()
{
  return s_version;
}

const char * MirandaState::value(const char * key)
{
  lock.lockForRead();
  MirandaCache * cache = s_cache->value(key, 0);
  lock.unlock();

  if( cache == 0 ) {
    return 0;
  }

  if ( cache->isExpires() ) {
    lock.lockForWrite();
    s_cache->remove(key);
    lock.unlock();
    return 0;
  } else {
    return cache->data();
  }

}

void MirandaState::insert(const char * key, const char * value)
{
  insert(key, value, -1);
}

void MirandaState::insert(const char * key, const char * value, int expires)
{
  lock.lockForWrite();
  s_cache->insert(key, new MirandaCache(value, expires));
  lock.unlock();
}

int MirandaState::remove(const char * key)
{
  lock.lockForWrite();
  return s_cache->remove(key);
  lock.unlock();
}
