// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include "mirandastate.h"
#include <QDebug>
#include <QFile>
#include <CByteArray>
#include <QList>

QMutex MirandaService::s_mutex;

void miranda_cache_register(lua_State * L);
void miranda_server_register(lua_State * L);
void miranda_service_register(lua_State * L);
void miranda_task_register(lua_State * L);

MirandaService::MirandaService(QByteArray fileName)
{
  m_fileName   = fileName;
  m_shutdown   = false;
  m_service    = true;
  m_uuid       = NULL;
  m_time       = os::microtime();
}

MirandaService::MirandaService(QByteArray fileName, QByteArray uuid)
{
  m_fileName   = fileName;
  m_shutdown   = false;
  m_service    = false;
  m_uuid       = uuid;
  m_time       = os::microtime();
}


MirandaService::~MirandaService()
{
  qDebug() << "destructor service ..." << m_fileName;
  MirandaState::s_cache->remove(m_uuid.data());
}

bool MirandaService::loop(int secs)
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

bool MirandaService::isShutdown()
{
  QMutexLocker ml(&s_mutex);

  if(m_shutdown) {
    return true;
  }

  return false;
}

void MirandaService::run() {

  int rv;
  MirandaState * state;
  lua_State * luaState;

  state = MirandaState::pop();
  luaState = state->instance();

  /* CHARON_TASK */
  lua_pushstring(luaState, m_uuid);
  lua_setglobal(luaState, "CHARON_TASK");

  //allocate
  lua_pushlightuserdata(luaState, this);
  lua_setglobal(luaState, "__miranda_service");

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

  // clear this
  lua_pushnil(luaState);
  lua_setglobal(luaState, "__miranda_service");

  // lua gc
  lua_gc(luaState, LUA_GCCOLLECT, 0);

  // stack push lua state
  MirandaState::push(state);

  // close lua state
  //lua_close(luaState);
  /*
  //debug cache
  int total = 0;
  QList<CByteArray> list = MirandaState::s_cache->keys();
  qDebug() << "TOTAL DE ITENS NO CACHE " << list.size() << "\n\n";

  for(int i = 0; i < list.size(); i++ ) {
    CByteArray key   = list.at(i);
    CByteArray value = MirandaState::s_cache->value(key);
    qDebug() << "key " << key << '\n';
    total += value.size();
  }

  qDebug() << "TOTAL CACHE ====================================\n\n";
  qDebug() << total << "\n\n\n";
  */

  if( m_service && QFile::exists( m_fileName ) ) {
    MirandaState::createService(m_fileName);
  }
}

void MirandaService::shutdown()
{
  QMutexLocker ml(&s_mutex);
  qDebug() << "shutdown" << m_fileName;
  m_shutdown = true;
}

double MirandaService::time()
{
  return m_time;
}
