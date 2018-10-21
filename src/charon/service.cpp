// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/cache>
#include <charon/mvm>
#include <charon/service>

using namespace charon;
using charon::cache;
using charon::mvm;

char * service::start(const char * fileName)
{
  char * uuid = os::uuid();

  char * uuidCopy = new char[strlen(uuid)+1];
  strcpy(uuidCopy, uuid);
  char * fileNameCopy = new char[strlen(fileName)+1];
  strcpy(fileNameCopy, fileName);

  new std::thread(service::run, uuidCopy, fileNameCopy);

  return uuid;
}

service::service()
{
  m_version = mvm::version();
}

bool service::loop(int secs)
{
  int i = 0;

  while( i < secs ) {

    os::sleep(1);

    if(m_version != mvm::version()) {
      return false;
    }

    i++;
  }

  return true;
}

void service::run(char * uuid, char * fileName)
{

  while( true )
  {

    {

    int rv;

    charon::instance i = mvm::instance();
    lua_State * L = i.state();
    lua_settop(L, 0);

    // global __charon_service
    lua_pushlightuserdata(L, new service());
    lua_setglobal(L, "__charon_service");

    // CHARON_UUID
    lua_pushstring(L, uuid);
    lua_setglobal(L, "CHARON_UUID");

    //TODO .lua end file use dofile not use require ???
    lua_getglobal(L, "dofile");
    lua_pushstring(L, fileName);

    rv = lua_pcall(L, 1, 0, 0);
    if (rv) {
      fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
    }

    // clear global __charon_service
    lua_getglobal(L, "__charon_service");
    service * srv = (service *) lua_touserdata(L, -1);
    lua_pushnil(L);
    lua_setglobal(L, "__charon_service");
    delete srv;

    // clear CHARON_TASK
    lua_pushboolean(L, false);
    lua_setglobal(L, "CHARON_UUID");

    // lua gc
    lua_gc(L, LUA_GCCOLLECT, 0);

    }

    // waiting for initialize next loop service
    os::sleep(1);
    std::cout << "reinitialize service " << fileName << std::endl;

  } // while

}
