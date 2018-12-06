// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/mvm>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <map>

using namespace charon;
using charon::ByteArray;

int        mvm::s_argc         = 0;
char **    mvm::s_argv         = 0;
int        mvm::s_gc           = 0;
int        mvm::s_version      = 0;
int        mvm::s_pool         = 0;
double     mvm::s_uptime       = os::microtime();
ByteArray  mvm::s_charonPath   = "";
ByteArray  mvm::s_profilePath  = "";
ByteArray  mvm::s_dispatchPath = "";
static std::mutex mtx;
static std::map <std::string, int> s_config;

void mvm_pool()
{
  while( true ) {
    int log = mvm::at("pool.log");
    if( log ) {
      mvm::log("pool...\n");
    }
    mtx.lock();
    int count = mvm::at("pool.size") - mvm::s_pool;
    mtx.unlock();
    if (count > 0) {
      for(int i=0; i < count; i++) {
        mvm::push( new mvm::data(mvm::s_version) );
      }
    }
    mvm::pause("pool.pause");
  }
}

void mvm_gc()
{
  while( true ) {
    int log = mvm::at("gc.log");
    if( log ) {
      mvm::log("gc ...\n");
    }
    mvm::gc();
    mvm::pause("gc.pause");
  }
}

void mvm::set(std::string key, int value)
{
  mtx.lock();
  s_config[key] = value;
  mtx.unlock();
}

int mvm::at(std::string key)
{
  return s_config[key];
}

bool mvm::pause(std::string key)
{
  int value = 0;
  while( true ) {
    os::sleep(1);
    value ++;
    int pause = mvm::at(key);
    if ( value >= pause ) {
      break;
    }
  }
  return false;
}


void mvm::log(const char * value)
{
  mtx.lock();
  std::cout << value;
  mtx.unlock();
}

void mvm::config()
{

  // defaults values
  mvm::set("gc.pause",   60);
  mvm::set("gc.log",      0);
  mvm::set("pool.pause", 30);
  mvm::set("pool.size",   5);
  mvm::set("pool.log",    0);

  const char * fileName = "config/mvm.lua";
  if( os::exists(fileName) ) {
    charon::instance i = mvm::instance();
    lua_State * L = i.state();
    lua_settop(L, 0);
    lua_getglobal(L, "dofile");
    lua_pushstring(L, fileName);
    int rv = lua_pcall(L, 1, 1, 0);
    if (rv) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
      throw;
    }
  }
}

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

  container::init();

  mvm::config();

  new std::thread(mvm_pool);
  new std::thread(mvm_gc);

}

instance mvm::instance()
{
  mvm::data * data  = pop();

  if( mvm::s_version != data->version() ) {
    delete data;
    data = new mvm::data(mvm::s_version);
  }

  return charon::instance(data);
}

void mvm::push(mvm::data * data)
{
  if( mvm::s_version != data->m_version ) {
    delete data;
  } else {
    mtx.lock();
    container::push(data);
    s_pool ++;
    mtx.unlock();
  }
}

void mvm::back(mvm::data * data)
{
  if( mvm::s_version > data->m_version ) {
    delete data;
  } else {
    mtx.lock();
    container::back(data);
    s_pool ++;
    mtx.unlock();
  }
}

mvm::data * mvm::pop()
{
  mtx.lock();
  if( container::empty() ) {
    mtx.unlock();
    return new mvm::data(mvm::s_version);
  }
  mvm::data * data = container::pop();
  s_pool--;
  mtx.unlock();
  return data;
}

void mvm::reload()
{
  int log = mvm::at("pool.log");
  int version = mvm::s_version + 1;

  if( log ) {
    char buffer[30];
    sprintf(buffer, "reload: pool size %i\n", s_pool);
    mvm::log(buffer);
  }

  while( true ) {
    mvm::data * data = mvm::pop();
    if( data->version() == version ) {
      break;
    } else {
      if( log ) {
        mvm::log("mvm delete\n");
      }
      delete data;
      mvm::back(new mvm::data(version));
    }
  }
  mvm::s_version = version;
}

int mvm::version()
{
  return mvm::s_version;
}

int mvm::pool()
{
  return s_pool;
}

int mvm::gc()
{
  int i = 0;
  mvm::data * data;

  s_gc ++;
  data = pop();

  while( data->m_gc != s_gc ) {
    lua_gc(data->state(), LUA_GCCOLLECT, 0);
    data->m_gc = s_gc;

    mvm::back(data);
    i++;

    data = pop();
  }

  mvm::back(data);

  return i;
}

int mvm::clear()
{
  int result = 0;
  mtx.lock();
  mvm::s_version++;
  while( !container::empty() ) {
    result++;
    mvm::data * data = container::pop();
    s_pool--;
    delete data;
  }
  mtx.unlock();
  return result;
}

double mvm::uptime()
{
  return os::microtime() - mvm::s_uptime;
}

mvm::data::data(int version)
{
  int rv;
  m_version = version;
  m_gc      = s_gc;
  m_State   = luaL_newstate();

  luaL_openlibs(m_State);

  // CHARON_PATH
  lua_pushstring(m_State, s_charonPath);
  lua_setglobal(m_State, "CHARON_PATH");

  // CHARON_UUID
  lua_pushboolean(m_State, false);
  lua_setglobal(m_State, "CHARON_UUID");

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

  int log = mvm::at("pool.log");
  if( log ) {
    mvm::log("mvm create\n");
  }

}

mvm::data::~data()
{
  if( m_release == false ) {
    lua_close(m_State);
  }
}

lua_State * mvm::data::state()
{
  return m_State;
}

lua_State * mvm::data::release()
{
  m_release = true;
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
  if( m_data->m_release ) {
    delete m_data;
  } else {
    mvm::push(m_data);
  }
}

lua_State * instance::state()
{
  return m_data->state();
}

lua_State * instance::release()
{
  return m_data->release();
}
