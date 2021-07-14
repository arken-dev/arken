#include <arken/concurrent/task/scheduled.h>
#include <arken/os.h>
#include <cstdio>
#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/mvm>
#include <arken/string.h>

namespace arken {
namespace concurrent {
namespace task {

std::mutex scheduled::s_mutex;
std::atomic<uint32_t> scheduled::s_max{mvm::threads()};
std::atomic<uint32_t> scheduled::s_actives{0};

scheduled::scheduled()
{
  scheduled::s_actives++;
}

scheduled::~scheduled()
{
  scheduled::s_actives--;
}

std::queue<scheduled::node> & scheduled::queue()
{
  static std::queue<scheduled::node> s_queue;
  return s_queue;
}

std::vector<string> & scheduled::vector()
{
  static std::vector<string> s_vector;
  return s_vector;
}

std::atomic<size_t> & scheduled::position()
{
  static std::atomic<size_t> s_position{0};
  return s_position;
}

std::unordered_map<string, std::queue<scheduled::node>> & scheduled::map()
{
  static std::unordered_map<string, std::queue<scheduled::node>> s_map;
  return s_map;
}

std::unordered_map<string, std::atomic<uint32_t>> & scheduled::runners()
{
  static std::unordered_map<string, std::atomic<uint32_t>> s_map;
  return s_map;
}

void scheduled::run()
{
  while (true) {
    scheduled::node node = dequeue();
    if( !node ) {
      break;
    }
    node.run();
  }
}

bool scheduled::release()
{
  return true;
}

scheduled::node scheduled::start(const char * fileName, const char * params, const char * name, bool purge)
{
  std::unique_lock<std::mutex> lck(scheduled::s_mutex);
  scheduled::node node = scheduled::node(fileName, params, name, purge);
  scheduled::push( node );

  if(scheduled::s_actives < scheduled::s_max) {
    mvm::concurrent( new scheduled() );
  }

  return node;
}

scheduled::node::node()
{}

scheduled::node::node(const node &obj)
{
  m_uuid      = obj.m_uuid;
  m_fileName  = obj.m_fileName;
  m_params    = obj.m_params;
  m_name      = obj.m_name;
  m_microtime = obj.m_microtime;
  m_shared    = obj.m_shared;
  m_ref_bool  = obj.m_ref_bool;
}

scheduled::node::node(const char * fileName, const char * params, const char * name, bool purge)
{
  m_fileName  = fileName;
  m_params    = params;
  m_name      = name;
  m_purge     = purge;
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
  m_ref_bool  = std::shared_ptr<std::atomic<bool>>(new std::atomic<bool>(false));
}

scheduled::node::~node()
{ }

void scheduled::node::run()
{
  int rv;
  arken::instance i = mvm::instance(m_purge);
  lua_State * L = i.state();
  lua_settop(L, 0);

  lua_getglobal(L,  "require");
  lua_pushstring(L, "arken.concurrent.task.scheduled");
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
  scheduled::node **ptr = (scheduled::node **)lua_newuserdata(L, sizeof(scheduled::node*));
  *ptr = new scheduled::node(*this);
  luaL_getmetatable(L, "arken.concurrent.task.scheduled.node.metatable");
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

  // decrease runners
  std::unique_lock<std::mutex> lck(scheduled::s_mutex);
  runners()[m_name]--;
  if( runners()[m_name] == 0 && map()[m_name].size() == 0 ) {
    map().erase(m_name);
    runners().erase(m_name);
    for(size_t p=0; p < scheduled::vector().size(); p++ ) {
      if( scheduled::vector().at(p) == m_name ) {
        scheduled::vector().erase(scheduled::vector().begin()+p);
        break;
      }
    }
    if( scheduled::position() >= scheduled::vector().size() ) {
      scheduled::position() = 0;
    }
  }

  (*m_ref_bool.get()) = true;
}

void scheduled::push(const scheduled::node & node)
{
  // TODO resolve .data() map arken:string
  if( scheduled::map().count(node.m_name) == 0 ) {
    scheduled::map()[node.m_name] = std::queue<scheduled::node>();
    scheduled::vector().push_back(node.m_name);
  }

  if( scheduled::runners().count(node.m_name) == 0 ) {
    runners()[node.m_name] = 0;
  }

  scheduled::map()[node.m_name.data()].push( node );
}

scheduled::node scheduled::dequeue()
{

  std::unique_lock<std::mutex> lck(scheduled::s_mutex);
  std::vector<string> &vector = scheduled::vector();
  std::unordered_map<string, std::queue<scheduled::node>> &map = scheduled::map();

  if( vector.empty() ) {
    return {};
  }

  string name = vector.at(position());
  string current;

  // for actual position to end vector size
  for(size_t pos = position(); pos < vector.size(); pos++) {

    current = vector.at(pos);

    if (runners()[current] < runners()[name]) {
      name = current;
    }

  } // for

  // for initial vector to position
  for(size_t pos = 0; pos < position(); pos++) {

    current = vector.at(pos);

    if (runners()[current] < runners()[name]) {
      name = current;
    }

  } // for

  scheduled::position()++;
  if( scheduled::position() >= vector.size() ) {
    scheduled::position() = 0;
  }

  if( map[name].empty() ) {
    return {};
  } else {
    // increase runners
    runners()[name]++;

    scheduled::node n = map[name].front();
    map[name].pop();

    return n;
  }

}

void scheduled::wait()
{
  mvm::wait();
}

string scheduled::node::uuid()
{
  return m_uuid;
}

string scheduled::node::name()
{
  return m_name;
}

double scheduled::node::microtime()
{
  return m_microtime;
}

scheduled::node::operator bool() const {
  return m_microtime > 0;
}

Shared scheduled::node::shared()
{
  return m_shared;
}

bool scheduled::node::finished()
{
  return (*m_ref_bool.get()) == true;
}

}  // namespace task
}  // namespace concurrent
}  // namespace arken
