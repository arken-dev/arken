// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/mvm>
#include <QStack>
#include <QMutex>

using namespace charon;
using charon::ByteArray;

int        mvm::s_gc           = 0;
int        mvm::s_version      = 0;
ByteArray  mvm::s_charonPath   = "";
ByteArray  mvm::s_profilePath  = "";
ByteArray  mvm::s_dispatchPath = "";
QMutex     mvm::s_mutex;
std::deque<mvm::data *> * mvm::s_container   = new std::deque<mvm::data *>;

void mvm::init(QCoreApplication *app)
{
  s_charonPath = app->applicationDirPath().toLocal8Bit();
  s_charonPath.truncate( s_charonPath.lastIndexOf('/') );

  s_profilePath = s_charonPath;
  s_profilePath.append("/profile.lua");

  s_dispatchPath.append("dispatch.lua");
}

instance mvm::instance()
{
  QMutexLocker ml(&s_mutex);
  mvm::data * data ;

  if( s_container->empty() ) {
    data = new mvm::data();
  } else {
    data = s_container->front();
    s_container->pop_front();
    if( s_version != data->version() ) {
      delete data;
      data = new mvm::data();
    }
  }

  return charon::instance(data);
}

void mvm::push(mvm::data * data)
{
  if( s_version != data->m_version ) {
      delete data;
  } else {
    QMutexLocker ml(&s_mutex);
    s_container->push_front(data);
  }
}

mvm::data * mvm::takeFirst()
{
  QMutexLocker ml(&s_mutex);

  if( s_container->empty() ) {
    return new mvm::data();
  }
  mvm::data * data = s_container->front();
  s_container->pop_front();
  return data;
}

void mvm::reload()
{
  s_version++;
  mvm::clear();
}

int mvm::version()
{
  return s_version;
}

int mvm::gc()
{
  int i = 0;
  mvm::data * data;

  s_gc ++;
  data = takeFirst();

  while( data->m_gc != s_gc ) {
    lua_gc(data->state(), LUA_GCCOLLECT, 0);
    data->m_gc = s_gc;

    s_container->push_back(data);
    i++;

    data = takeFirst();
  }

  s_container->push_back(data);

  return i;
}

int mvm::clear()
{
  int result = 0;
  QMutexLocker ml(&s_mutex);
  s_version++;
  while( !s_container->empty() ) {
    result++;
    mvm::data * data = s_container->front();
    s_container->pop_front();
    delete data;
  }
  return result;
}

mvm::data::data()
{
  int rv;
  m_version = s_version;
  m_gc      = s_gc;
  m_State   = luaL_newstate();

  luaL_openlibs(m_State);

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

  if( os::exists(s_dispatchPath) ) {
    luaL_loadfile(m_State, s_dispatchPath );
    lua_pcall(m_State, 0, 0, 0);
  }

  qDebug() << "mvm create Lua State";
}

mvm::data::~data()
{
  lua_close(m_State);
}

lua_State * mvm::data::state()
{
  return m_State;
}

int mvm::data::version()
{
  return m_version;
}

charon::instance::instance(mvm::data * data)
{
  m_data = data;
}

charon::instance::~instance()
{
  mvm::push(m_data);
}

lua_State * instance::state()
{
  return m_data->state();
}
