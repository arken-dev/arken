#include <arken/concurrent/task/singular.h>
#include <arken/os.h>
#include <cstdio>
#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/mvm>
#include <arken/string.h>
#include <algorithm>

namespace arken {
namespace concurrent {
namespace task {

std::mutex singular::s_mutex;
std::atomic<uint32_t> singular::s_max{mvm::threads()};
std::atomic<uint32_t> singular::s_actives{0};

static std::mutex s_inspect_mutex;
static std::map<string, string> s_inspect_map;

singular::singular()
{
  m_uuid    = os::uuid();
  m_inspect = "arken.concurrent.task.singular";
  singular::s_actives++;
}

singular::~singular()
{
  singular::s_actives--;
}

std::queue<singular::node> & singular::queue()
{
  static std::queue<singular::node> s_queue;
  return s_queue;
}

std::vector<string> & singular::vector()
{
  static std::vector<string> s_vector;
  return s_vector;
}

std::atomic<size_t> & singular::position()
{
  static std::atomic<size_t> s_position{0};
  return s_position;
}

std::unordered_map<string, std::queue<singular::node>> & singular::map()
{
  static std::unordered_map<string, std::queue<singular::node>> s_map;
  return s_map;
}

std::unordered_map<string, std::atomic<bool>> & singular::runners()
{
  static std::unordered_map<string, std::atomic<bool>> s_map;
  return s_map;
}

void singular::run()
{
  while (true) {
    singular::node node = dequeue();
    if( !node ) {
      break;
    }
    string tmp;
    tmp.append(node.m_fileName).append("#").append(node.params()).append("#").append(node.name());
    s_inspect_mutex.lock();
    s_inspect_map[node.uuid()] = tmp;
    s_inspect_mutex.unlock();
    node.run();
    s_inspect_mutex.lock();
    s_inspect_map.erase(node.uuid());
    s_inspect_mutex.unlock();
  }
}

bool singular::release()
{
  return true;
}

singular::node singular::start(const char * fileName, const char * params, const char * name, bool purge)
{
  std::unique_lock<std::mutex> lck(singular::s_mutex);

  singular::node node = singular::node(fileName, params, name, purge);
  singular::push( node );

  if(singular::s_actives < singular::runners().size() && singular::s_actives < singular::s_max) {
    mvm::concurrent( new singular() );
  }

  return node;
}

singular::node singular::emplace(const char * fileName, const char * params, const char * name, bool purge)
{
  std::unique_lock<std::mutex> lck(singular::s_mutex);

  if( singular::map().count(name) == 0 || singular::map()[name].empty() ) {

    singular::node node = singular::node(fileName, params, name, purge);
    singular::push( node );

    if(singular::s_actives < singular::runners().size() && singular::s_actives < singular::s_max) {
      mvm::concurrent( new singular() );
    }

    return node;
  } //if

  return {};
}

singular::node singular::place(const char * fileName, const char * params, const char * name, bool purge)
{
  std::unique_lock<std::mutex> lck(singular::s_mutex);

  if( singular::runners().count(name) == 0 ) {

    singular::node node = singular::node(fileName, params, name, purge);
    singular::push( node );

    if(singular::s_actives < singular::runners().size() && singular::s_actives < singular::s_max) {
      mvm::concurrent( new singular() );
    }

    return node;
  } //if

  return {};
}


singular::node::node()
{}

singular::node::node(const node &obj)
{
  m_uuid      = obj.m_uuid;
  m_fileName  = obj.m_fileName;
  m_params    = obj.m_params;
  m_name      = obj.m_name;
  m_microtime = obj.m_microtime;
  m_shared    = obj.m_shared;
  m_ref_bool  = obj.m_ref_bool;
}

singular::node::node(const char * fileName, const char * params, const char * name, bool purge)
{
  m_fileName  = fileName;
  m_params    = params;
  m_name      = name;
  m_purge     = purge;
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
  m_ref_bool  = std::shared_ptr<std::atomic<bool>>(new std::atomic<bool>(false));
}

singular::node::~node()
{ }

void singular::node::run()
{
  int rv;
  arken::instance i = mvm::instance(m_purge);
  lua_State * L = i.state();
  lua_settop(L, 0);

  lua_getglobal(L,  "require");
  lua_pushstring(L, "arken.concurrent.task.singular");
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
  singular::node **ptr = (singular::node **)lua_newuserdata(L, sizeof(singular::node*));
  *ptr = new singular::node(*this);
  luaL_getmetatable(L, "arken.concurrent.task.singular.node.metatable");
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

  (*m_ref_bool.get()) = true;
  runners()[m_name]   = true;

  std::unique_lock<std::mutex> lck(singular::s_mutex);

  if( map()[m_name].size() == 0 ) {
    map().erase(m_name);
    runners().erase(m_name);

    for(size_t p=0; p < singular::vector().size(); p++ ) {
      if( singular::vector().at(p) == m_name ) {
        singular::vector().erase(singular::vector().begin()+p);
        break;
      }
    }
    if( singular::position() >= singular::vector().size() ) {
      singular::position() = 0;
    }
  }


}

void singular::push(const singular::node & node)
{
  // TODO resolve .data() map arken:string
  if( singular::map().count(node.m_name) == 0 ) {
    runners()[node.m_name] = true;
    singular::map()[node.m_name] = std::queue<singular::node>();
    singular::vector().push_back(node.m_name);
  }

/*
  if( singular::runners().count(node.m_name) == 0 ) {
    runners()[node.m_name] = true;
  }
*/

  singular::map()[node.m_name].push( node );
}

singular::node singular::dequeue()
{

  std::unique_lock<std::mutex> lck(singular::s_mutex);
  std::vector<string> &vector = singular::vector();
  std::unordered_map<string, std::queue<singular::node>> &map = singular::map();

  if( vector.empty() ) {
    return {};
  }

  if( s_actives > vector.size() ) {
    return {};
  }

  string name;
  string current;

  while( true ) {

    // for actual position to end vector size
    for(size_t pos = position(); pos < vector.size(); pos++) {

      current = vector.at(pos);

      if (runners()[current]) {
        name = current;
        break;
      }

    } // for

    if(! name.empty()) {
      break;
    }

    // for initial vector to position
    for(size_t pos = 0; pos < position(); pos++) {

      current = vector.at(pos);

      if (runners()[current]) {
        name = current;
        break;
      }

    } // for

    if(! name.empty()) {
      break;
    }

    // wait find
    os::sleep(1);

  } // while

  singular::position()++;
  if( singular::position() >= vector.size() ) {
    singular::position() = 0;
  }

  if( map[name].empty() ) {
    return {};
  } else {
    // flag runners
    runners()[name] = false;

    singular::node n = map[name].front();
    map[name].pop();

    return n;
  }

}

void singular::wait()
{
  mvm::wait();
}

string singular::node::uuid()
{
  return m_uuid;
}

string singular::node::name()
{
  return m_name;
}

string singular::node::params()
{
  return m_params;
}

double singular::node::microtime()
{
  return m_microtime;
}

singular::node::operator bool() const {
  return m_microtime > 0;
}

Shared singular::node::shared()
{
  return m_shared;
}

bool singular::node::finished()
{
  return (*m_ref_bool.get()) == true;
}

void singular::node::wait()
{
  while ((*m_ref_bool.get()) == false) {
    os::sleep(0.05);
  }
}

uint32_t singular::actives()
{
  return s_actives;
}

string singular::inspect()
{
  string tmp("[");
  s_inspect_mutex.lock();
  for (std::pair<string, string> element : s_inspect_map) {
    if( tmp.size() > 1 ) {
      tmp.append(",");
    }
    tmp.append("\"").append(element.second).append("\"");
  }
  s_inspect_mutex.unlock();
  tmp.append("]");
  return tmp;
}

}  // namespace task
}  // namespace concurrent
}  // namespace arken
