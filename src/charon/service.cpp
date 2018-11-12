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

int           service::s_version  = mvm::version();
char        * service::s_dirName  = 0;
std::mutex  * service::s_mutex    = new std::mutex;
std::vector<std::string> * service::s_services = new std::vector<std::string>;

void service::load(const char * dirName)
{
  if( ! s_dirName ) {
    s_dirName = new char(strlen(dirName)+1);
    strcpy(s_dirName, dirName);
  }
  ByteArrayList * list = os::glob(dirName, ".lua$");
  for( int i = 0; i < list->size(); i++ ) {
    service::start(list->at(i));
  }
  delete list;
}

void service::load()
{
  if( s_dirName ) {
    service::load(s_dirName);
  } else {
    std::cout << "warning charon::service dirName not initialize\n" << std::endl;
  }
}

char * service::start(const char * fileName)
{
  service::s_mutex->lock();
  bool flag = std::find(s_services->begin(), s_services->end(), std::string(fileName)) != s_services->end();
  service::s_mutex->unlock();
  //std::unique_lock<std::mutex> lck(*service::s_mutex);
  if(flag) {
    // service included for list return invalid uuid;
    return 0;
  } else {
    std::cout << "start service " << fileName << std::endl;
    // uuid for comunication service and app
    char * uuid = os::uuid();

    // copy string for service thread
    char * uuidCopy = new char[strlen(uuid)+1];
    strcpy(uuidCopy, uuid);
    char * fileNameCopy = new char[strlen(fileName)+1];
    strcpy(fileNameCopy, fileName);

    // start thread (service)
    new std::thread(service::run, uuidCopy, fileNameCopy);

    // push back fileName
    service::s_mutex->lock();
    s_services->push_back(std::string(fileName));
    service::s_mutex->unlock();

    return uuid;
  }
}

service::service()
{
  m_quit    = false;
  m_version = mvm::version();
}

void service::quit()
{
  m_quit = true;
}

bool service::loop(int secs)
{
  int i = 0;

  while( i < secs ) {

    os::sleep(1);

    if(m_quit || m_version != mvm::version()) {
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

    int rv;

    charon::instance i = mvm::instance();
    lua_State * L = i.state();
    lua_settop(L, 0);

    // global __charon_service
    service * srv = new service();
    lua_pushlightuserdata(L, srv);
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
    lua_pushnil(L);
    lua_setglobal(L, "__charon_service");

    // clear CHARON_TASK
    lua_pushboolean(L, false);
    lua_setglobal(L, "CHARON_UUID");

    // lua gc
    lua_gc(L, LUA_GCCOLLECT, 0);

    // check new services
    service::s_mutex->lock();
    if( service::s_version != mvm::version() ) {
      service::s_version = mvm::version();
      service::s_mutex->unlock();
      std::cout << "check new services" << std::endl;
      service::load();
    } else {
      service::s_mutex->unlock();
    }

    // check (service) fileName exists
    if (srv->m_quit == false && os::exists(fileName)) {
      // waiting for initialize next loop service
      os::sleep(1);
      std::cout << "restart service " << fileName << std::endl;
      delete srv;
    } else {
      std::cout << "quit service " << fileName << std::endl;
      service::s_mutex->lock();
      s_services->erase(
        remove(s_services->begin(), s_services->end(), std::string(fileName)), s_services->end()
      );
      service::s_mutex->unlock();
      delete[] fileName;
      delete[] uuid;
      delete srv;
      return;
    }

  } // while

}
