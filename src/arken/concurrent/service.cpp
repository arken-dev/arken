#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/concurrent/service.h>
#include <arken/concurrent/core.h>
#include <arken/os.h>
#include <arken/string.h>
#include <arken/json.h>

using List = arken::string::List;

namespace arken {
namespace concurrent {

std::atomic<uint32_t> service::s_version(mvm::version());
std::unordered_map<string, bool> service::s_references;
std::vector<string> service::s_dirName;
std::mutex service::s_mutex;

service::service( const char * fileName, const char * params, bool purge )
{
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
  m_version   = mvm::version();
  m_fileName  = fileName;
  m_params    = params;
  m_purge     = purge;
  m_inspect.
    append("arken.concurrent.service: ").
    append(m_fileName).append("#").
    append(m_params.escape());

  s_references[fileName] = true;
}

service::service(const service &obj)
{
  m_version   = mvm::version();
  m_fileName  = obj.m_fileName;
  m_params    = obj.m_params;
  m_purge     = obj.m_purge;
  m_inspect   = obj.m_inspect;
  m_shared    = obj.m_shared;
  m_uuid      = obj.m_uuid;
  m_microtime = obj.m_microtime;
}

service::~service() = default;

service service::start(const char * fileName, const char * params, bool purge)
{
  auto ptr = new service(fileName, params, purge);
  core::start(ptr);
  return service(*ptr);
}

void service::run()
{
  int rv;
  mvm::instance i = mvm::getInstance( m_purge );
  i.swap(m_shared);

  lua_State * L = i.state();
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

  json::decode(L, m_params);

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

  if( checkReload() ) {
    std::cout << "reloading service ..." << std::endl;
    service::reload();
  }

  os::sleep(1);
  if (os::exists(m_fileName)) {
    core::start(new service(*this));
  } else {
    std::cout << "erase service " << m_fileName << std::endl;
    s_references.erase(m_fileName);
  }
} //run

bool service::checkReload()
{
  std::unique_lock<std::mutex> lck(service::s_mutex);

  bool result = false;
  if( service::s_version < mvm::version() ) {
    service::s_version = mvm::version();
    result = true;
  }

  return result;
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

void service::reload()
{
  for( size_t i=0; i < s_dirName.size(); i++ ) {
    service::run(s_dirName[i]);
  }
}

void service::run(const char * dirName)
{

  List list = os::find(dirName, ".lua$");

  for( int i = 0; i < list.size(); i++ ) {
    std::cout << "start service " << list[i] << std::endl;
    if( service::s_references.count( list[i] ) == 0 ) {
      service::start(list[i], "{}", false);
    }
  }
}

void service::load(const char * dirName)
{

  std::cout << "load " << dirName << std::endl;
  if( ! os::exists(dirName) ) {
    std::cout << "not exists " << dirName << std::endl;
    return;
  }

  s_dirName.push_back(dirName);
  service::run(dirName);

}

} // namespace concurrent
} // namespace arken
