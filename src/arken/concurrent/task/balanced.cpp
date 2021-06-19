#include <arken/concurrent/task/balanced.h>
#include <arken/os.h>
#include <cstdio>
#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/mvm>
#include <arken/string.h>


namespace arken {
namespace concurrent {
namespace task {

using mvm    = arken::mvm;
using string = arken::string;

std::mutex balanced::s_mutex;
std::atomic<int> balanced::s_max{1};
std::atomic<int> balanced::s_actives{0};

balanced::balanced()
{
  balanced::s_actives++;
}

balanced::~balanced()
{
  balanced::s_actives--;
}

std::queue<balanced::node> & balanced::queue()
{
  static std::queue<balanced::node> s_queue;
  return s_queue;
}

std::vector<string> & balanced::vector()
{
  static std::vector<string> s_vector;
  return s_vector;
}

std::atomic<size_t> & balanced::position()
{
  static std::atomic<size_t> s_position{0};
  return s_position;
}

std::unordered_map<string, std::queue<balanced::node>> & balanced::map()
{
  static std::unordered_map<string, std::queue<balanced::node>> s_map;
  return s_map;
}

void balanced::run()
{
  while (true) {
    balanced::node node = dequeue();
    if( !node ) {
      break;
    }
    node.run();
  }
}

bool balanced::release()
{
  return true;
}

balanced::node balanced::start(const char * fileName, const char * params, const char * name, bool purge)
{
  std::unique_lock<std::mutex> lck(balanced::s_mutex);
  balanced::node node = balanced::node(fileName, params, name, purge);
  balanced::push( node );

  if(balanced::s_actives < balanced::s_max) {
    mvm::concurrent( new balanced() );
  }

  return node;
}

balanced::node::node()
{}

balanced::node::node(const node &obj)
{
  m_uuid      = obj.m_uuid;
  m_fileName  = obj.m_fileName;
  m_params    = obj.m_params;
  m_name      = obj.m_name;
  m_microtime = obj.m_microtime;
  m_shared    = obj.m_shared;
}

balanced::node::node(const char * fileName, const char * params, const char * name, bool purge)
{
  m_fileName  = fileName;
  m_params    = params;
  m_name      = name;
  m_purge     = purge;
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
}

void balanced::node::run()
{
  int rv;
  arken::instance i = mvm::instance(m_purge);
  lua_State * L = i.state();
  lua_settop(L, 0);

  lua_getglobal(L,  "require");
  lua_pushstring(L, "arken.concurrent.task.balanced");
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

  //lua_pushlstring(L,  m_uuid, 37);
  balanced::node **ptr = (balanced::node **)lua_newuserdata(L, sizeof(balanced::node*));
  *ptr = new balanced::node(*this);
  luaL_getmetatable(L, "arken.concurrent.task.balanced.node.metatable");
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
}

void balanced::push(const balanced::node & node)
{
  // TODO resolve .data() map arken:string
  if( balanced::map().count(node.m_name.data()) == 0 ) {
    balanced::map()[node.m_name.data()] = std::queue<balanced::node>();
    balanced::vector().push_back(node.m_name.data());
  }
  balanced::map()[node.m_name.data()].push( node );
}

balanced::node balanced::dequeue()
{
  std::unique_lock<std::mutex> lck(balanced::s_mutex);
  string name;
  std::vector<string> &vector = balanced::vector();
  std::unordered_map<string, std::queue<balanced::node>> &map = balanced::map();

  while( true ) {

    if( vector.empty() ) {
      name = "";
      break;
    }

    if( position() >= vector.size() ) {
      position() = 0;
    }

    name = vector.at(position());

    if (map[name].empty()) {
      map.erase(name);
      vector.erase(balanced::vector().begin()+position());
    } else {
      break;
    }

  } // while

  position()++;

  if( name.empty() ) {
    return {};
  } else {
    balanced::node n = map[name].front();
    map[name].pop();
    return n;
  }
}

void balanced::wait()
{
  mvm::wait();
}

string balanced::node::uuid()
{
  return m_uuid;
}

string balanced::node::name()
{
  return m_name;
}

double balanced::node::microtime()
{
  return m_microtime;
}

balanced::node::operator bool() const {
  return m_microtime > 0;
}

Shared balanced::node::shared()
{
  return m_shared;
}


}  // namespace task
}  // namespace concurrent
}  // namespace arken
