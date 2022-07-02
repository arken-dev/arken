#include <arken/concurrent/task/balanced.h>
#include <arken/os.h>
#include <cstdio>
#include <lua/lua.hpp>
#include <arken/string.h>
#include <arken/json.h>
#include <arken/concurrent/core.h>

namespace arken {
namespace concurrent {
namespace task {

balanced::balanced()
{
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
  balanced::actives()++;

  m_shared.name("arken.concurrent.task.balanced");
}

balanced::~balanced() = default;

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

    this->swap(node.shared());
    node.run();

    std::unique_lock<std::mutex> lck(balanced::mutex());
    running().erase(node.uuid());

  }
}

balanced::node balanced::start(const char * fileName, const char * params, const char * name, bool release)
{
  std::unique_lock<std::mutex> lck(balanced::mutex());
  balanced::node node = balanced::node(fileName, params, name, release);
  balanced::push( node );

  if(balanced::actives() < balanced::max()) {
    core::start(new balanced());
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
  m_finished  = obj.m_finished;
  m_release   = obj.m_release;
}

balanced::node::node(const char * fileName, const char * params, const char * name, bool release)
{
  m_uuid      = os::uuid();
  m_fileName  = fileName;
  m_params    = params;
  m_name      = name;
  m_release   = release;
  m_microtime = os::microtime();
  m_finished  = std::shared_ptr<std::atomic<bool>>(new std::atomic<bool>(false));
  m_shared.name("arken.concurrentask.task.balanced#");
  m_shared.name().append(m_name);
}

void balanced::node::run()
{
  int rv;
  mvm::instance instance = mvm::getInstance(m_release);
  instance.swap(m_shared);

  lua_State * L = instance.state();
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
  auto ptr = static_cast<balanced::node **>(lua_newuserdata(L, sizeof(balanced::node*)));
  *ptr = new balanced::node(*this);
  luaL_getmetatable(L, "arken.concurrent.task.balanced.node.metatable");
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

  (*m_finished.get()) = true;
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
  std::unique_lock<std::mutex> lck(balanced::mutex());
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
    balanced::actives()--;
    return {};
  } else {
    balanced::node n = map[name].front();
    map[name].pop();

    running()[n.uuid()] = n.inspect();
    return n;
  }
}

string balanced::node::name()
{
  return m_name;
}

std::unordered_map<string, string> &balanced::running()
{
  static std::unordered_map<string, string> s_running;
  return s_running;
}

string balanced::inspect()
{
  std::unique_lock<std::mutex> lck(balanced::mutex());

  string tmp("{");

  tmp.append("\"running\": [");
  int c = 0;
  for (std::pair<string, string> element : running()) {
    if( c > 0 ) {
      tmp.append(",");
    }
    tmp.append("\"").append(element.second).append("\"");
    c++;
  }
  tmp.append("],");
  tmp.append("\"queues\": {");

  c = 0;
  for (std::pair<string, std::queue<balanced::node>> element : balanced::map()) {
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

std::mutex & balanced::mutex()
{
  static std::mutex s_mutex;
  return s_mutex;
}

std::atomic<uint32_t> & balanced::actives()
{
  static std::atomic<uint32_t> s_actives{0};
  return s_actives;
}

std::atomic<uint32_t> & balanced::max()
{
  static std::atomic<uint32_t> s_max{core::max().load()};
  return s_max;
}

}  // namespace task
}  // namespace concurrent
}  // namespace arken
