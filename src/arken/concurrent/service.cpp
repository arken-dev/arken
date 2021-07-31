#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/concurrent/service.h>
#include <arken/os.h>
#include <arken/string.h>

using List = arken::string::List;

namespace arken {
namespace concurrent {

service::service( const char * fileName, const char * params, bool purge )
{
  m_version  = mvm::version();
  m_fileName = fileName;
  m_params   = params;
  m_purge    = purge;
  m_release  = true;
}

service::~service()
{

}

void service::start(const char * fileName, const char * params, bool purge)
{
  mvm::concurrent( new service(fileName, params, purge) );
}

void service::run()
{

  // if m_purge is true, create a new arken::instance
  // because it will be destroyed in the end
  arken::instance i = mvm::instance( m_purge );
  lua_State * L = i.state();
  lua_settop(L, 0);

  int rv;

  lua_settop(L, 0);

  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.concurrent.service");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  if( m_fileName.endsWith(".lua") ) {
    lua_getglobal(L, "dofile");
    lua_pushstring(L, m_fileName);
    rv = lua_pcall(L, 1, 1, 0);
    if (rv) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
    }
  } else {
    lua_getglobal(L, "require");
    lua_pushstring(L, m_fileName);
    rv = lua_pcall(L, 1, 1, 0);
    if (rv) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
    }
  }

  auto ptr = static_cast<service **>(lua_newuserdata(L, sizeof(service*)));
  *ptr = this;
  luaL_getmetatable(L, "arken.concurrent.service.metatable");
  lua_setmetatable(L, -2);

  json_lock_decode(L, m_params);

  rv = lua_pcall(L, 2, 0, 0); // alterar para envio de 1
  if (rv) {
    fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
  }

  // GC
  if( m_purge ) {
    i.release();
    lua_close(L);
  } else {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }

  os::sleep(1);
  mvm::concurrent( new service(m_fileName, m_params, m_purge) );
} //run

bool service::release()
{
  return m_release;
}

void service::wait()
{
  mvm::wait();
}

bool service::loop(int secs)
{
  int i = 0;

  while( i < secs ) {

    if( m_version != mvm::version() ) {
      return false;
    }

    os::sleep(1);
    i++;

  }

  return true;
}

void service::load(const char * dirName)
{
  List list = os::find(dirName, ".lua$");
  for( int i = 0; i < list.size(); i++ ) {
    std::cout << "start service " << list[i] << std::endl;
    service::start(list[i], "{}", false);
  }
}

} // namespace concurrent
} // namespace arken
