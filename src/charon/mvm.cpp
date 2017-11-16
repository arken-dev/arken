// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/mvm>
#include <cstring>
#include <iostream>
#include <mutex>

using namespace charon;
using charon::ByteArray;

int        mvm::s_argc         = 0;
char **    mvm::s_argv         = 0;
int        mvm::s_gc           = 0;
int        mvm::s_version      = 0;
ByteArray  mvm::s_charonPath   = "";
ByteArray  mvm::s_profilePath  = "";
ByteArray  mvm::s_dispatchPath = "";
std::deque<mvm::data *> * mvm::s_container = new std::deque<mvm::data *>;
std::mutex mtx;

void mvm::init(int argc, char ** argv)
{
  s_argc  = argc;
  s_argv  = new char*[argc+1];
  for( int i=0; i < argc; i++ ) {
    int len = strlen(argv[i]) + 1;
    s_argv[i] = new char[len]();
    strcpy(s_argv[i], argv[i]);
  }
  s_charonPath = os::executablePath();
  int lastIndexOf = s_charonPath.lastIndexOf("bin");
  s_charonPath.truncate(lastIndexOf-1);

  if( strcmp(os::name(), "windows") == 0 ) {
    s_charonPath = s_charonPath.capitalize();
  }

  s_profilePath = s_charonPath;
  s_profilePath.append("/profile.lua");

  s_dispatchPath.append("dispatch.lua");
}

instance mvm::instance()
{
  mvm::data * data ;

  mtx.lock();
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
  mtx.unlock();
  return charon::instance(data);
}

void mvm::push(mvm::data * data)
{
  if( s_version != data->m_version ) {
      delete data;
  } else {
    mtx.lock();
    s_container->push_front(data);
    mtx.unlock();
  }
}

mvm::data * mvm::takeFirst()
{
  mtx.lock();
  if( s_container->empty() ) {
    return new mvm::data();
  }
  mvm::data * data = s_container->front();
  s_container->pop_front();
  mtx.unlock();
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
  mtx.lock();
  s_version++;
  while( !s_container->empty() ) {
    result++;
    mvm::data * data = s_container->front();
    s_container->pop_front();
    delete data;
  }
  mtx.unlock();
  return result;
}

mvm::data::data()
{
  int rv;
  m_version = s_version;
  m_gc      = s_gc;
  m_State   = luaL_newstate();

  luaL_openlibs(m_State);

  lua_pushstring(m_State, s_charonPath);
  lua_setglobal(m_State, "CHARON_PATH");

  //---------------------------------------------
  int top, i;

  lua_settop(m_State, 0);
  lua_newtable(m_State);
  top = lua_gettop(m_State);
  for(i=1; i < s_argc; i++) {
    lua_pushinteger(m_State, i-1);
    lua_pushstring(m_State, s_argv[i]);
    lua_settable(m_State, top);
  }
  lua_setglobal(m_State, "arg");

  //---------------------------------------------
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

  std::cout << "mvm create Lua State\n";
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
