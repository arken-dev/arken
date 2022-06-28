#include <arken/concurrent/task/singular.h>
#include <arken/os.h>
#include <cstdio>
#include <lua/lua.hpp>
#include <arken/mvm.h>
#include <arken/string.h>
#include <algorithm>
#include <arken/json.h>
#include <arken/concurrent/core.h>

namespace arken {
namespace concurrent {
namespace task {

singular::singular()
{
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
  singular::actives()++;

  m_shared.name("arken.concurrent.task.singular");
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

    this->swap(node.shared());
    node.run();

    std::unique_lock<std::mutex> lck(singular::mutex());
    running().erase(node.uuid());
  }
}

singular::node singular::start(const char * fileName, const char * params, const char * name, bool release)
{
  std::unique_lock<std::mutex> lck(singular::mutex());

  singular::node node = singular::node(fileName, params, name, release);
  singular::push( node );

  if( singular::actives() < singular::max() && singular::actives() < singular::runners().size() ) {
    core::start(new singular());
  }

  return node;
}

singular::node singular::emplace(const char * fileName, const char * params, const char * name, bool release)
{
  std::unique_lock<std::mutex> lck(singular::mutex());

  if( singular::map().count(name) == 0 || singular::map()[name].empty() ) {

    singular::node node = singular::node(fileName, params, name, release);
    singular::push( node );

    if( singular::actives() < singular::max() ) {
      core::start(new singular());
    }

    return node;
  } //if

  return {};
}

singular::node singular::place(const char * fileName, const char * params, const char * name, bool release)
{
  std::unique_lock<std::mutex> lck(singular::mutex());

  if( runners().count(name) == 0 ) {

    singular::node node = singular::node(fileName, params, name, release);
    singular::push( node );

    if( singular::actives() < singular::max() ) {
      core::start(new singular());
    }

    return node;
  } //if

  return {};
}

singular::node::node() = default;

singular::node::node(const node &obj)
{
  m_uuid      = obj.m_uuid;
  m_fileName  = obj.m_fileName;
  m_params    = obj.m_params;
  m_name      = obj.m_name;
  m_microtime = obj.m_microtime;
  m_shared    = obj.m_shared;
  m_finished  = obj.m_finished;
  m_release     = obj.m_release;
}

singular::node::node(const char * fileName, const char * params, const char * name, bool release)
{
  m_fileName  = fileName;
  m_params    = params;
  m_name      = name;
  m_release     = release;
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
  m_finished  = std::shared_ptr<std::atomic<bool>>(new std::atomic<bool>(false));
  m_shared.name("arken.concurrent.task.singular#");
  m_shared.name().append(m_name.escape());
}

void singular::node::run()
{
  int rv;
  mvm::instance instance = mvm::getInstance(m_release);
  instance.swap(m_shared);

  lua_State * L = instance.state();
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
  auto ptr = static_cast<singular::node **>(lua_newuserdata(L, sizeof(singular::node*)));
  *ptr = new singular::node(*this);
  luaL_getmetatable(L, "arken.concurrent.task.singular.node.metatable");
  lua_setmetatable(L, -2);

  json::decode(L, m_params);

  rv = lua_pcall(L, 2, 0, 0); // alterar para envio de 1
  if (rv) {
    fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
  }


  // GC
  if( m_release ) {
    instance.release();
  } else {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }


  std::unique_lock<std::mutex> lck(singular::mutex());

  (*m_finished.get()) = true;

  if( map()[m_name].empty() ) {
    map().erase(m_name);
    runners().erase(m_name);

    for(size_t p=0; p < vector().size(); p++ ) {
      if( vector().at(p) == m_name ) {
        vector().erase(vector().begin()+p);
        break;
      }
    }
    if( position() > vector().size() ) {
      position() = 0;
    }
  } else {
    runners()[m_name] = true;
  }

}

void singular::push(const singular::node & node)
{
  if( map().count(node.m_name) == 0 ) {
    runners()[node.m_name] = true;
    map()[node.m_name] = std::queue<singular::node>();
    vector().push_back(node.m_name);
  }

  map()[node.m_name].push( node );
}

singular::node singular::dequeue()
{

  std::unique_lock<std::mutex> lck(singular::mutex());

  if( actives() > vector().size() ) {
    singular::actives()--;
    return {};
  }

  string name;
  string current;

  // for actual position to end vector size
  for(size_t pos = position(); pos < vector().size(); pos++) {

    current = vector().at(pos);

    if (runners()[current]) {
      name = current;
      break;
    }

  } // for

  if( name.empty() ) {

    // for initial vector to position
    for(size_t pos = 0; pos < position(); pos++) {

      current = vector().at(pos);

      if (runners()[current]) {
        name = current;
        break;
      }

    } // for

  }

  position()++;

  if( position() > vector().size() ) {
    position() = 0;
  }

  if( name.empty() || map()[name].empty() ) {
    singular::actives()--;
    return {};
  } else {
    runners()[name] = false;
    singular::node n = map()[name].front();
    map()[name].pop();
    running()[n.uuid()] = n;

    return n;
  }

}

string singular::node::name()
{
  return m_name;
}

string singular::inspect()
{
  std::unique_lock<std::mutex> lck(singular::mutex());

  string tmp("{");

  tmp.append("\"running\": [");
  int c = 0;
  for (std::pair<string, singular::node> element : running()) {
    if( c > 0 ) {
      tmp.append(",");
    }
    tmp.append("\"").append(element.second.inspect()).append("\"");
    c++;
  }
  tmp.append("],");
  tmp.append("\"queues\": {");

  c = 0;
  for (std::pair<string, std::queue<singular::node>> element : singular::map()) {
    if( c > 0 ) {
      tmp.append(",");
    }
    tmp.append("\"").append(element.first).append("\": ");
    tmp.append(std::to_string(element.second.size()));
    c++;
  }

  tmp.append("}");
  tmp.append("}");
  return tmp;
}

std::unordered_map<string, singular::node> &singular::running()
{
  static std::unordered_map<string, singular::node> s_running;
  return s_running;
}

std::mutex & singular::mutex()
{
  static std::mutex s_mutex;
  return s_mutex;
}

std::atomic<uint32_t> & singular::actives()
{
  static std::atomic<uint32_t> s_actives{0};
  return s_actives;
}

std::atomic<uint32_t> & singular::max()
{
  static std::atomic<uint32_t> s_max{core::max().load()};
  return s_max;
}

}  // namespace task
}  // namespace concurrent
}  // namespace arken
