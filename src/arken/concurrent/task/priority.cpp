#include <arken/concurrent/task/priority.h>
#include <arken/os.h>
#include <cstdio>
#include <lua/lua.hpp>
#include <arken/mvm.h>
#include <arken/json.h>


namespace arken {
namespace concurrent {
namespace task {

priority::priority()
{
  m_inspect   = "arken.concurrent.task.priority";
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
  priority::actives()++;
}

priority::~priority()
{
  priority::actives()--;
}

std::priority_queue<priority::node, std::vector<priority::node>, priority::node> & priority::priority_queue()
{
  static std::priority_queue<priority::node, std::vector<priority::node>, priority::node> s_priority_queue;
  return s_priority_queue;
}

void priority::run()
{
  while (true) {

    priority::node node = dequeue();

    if( !node ) {
      break;
    }

    swap(this, &node);
    node.run();

    std::unique_lock<std::mutex> lck(priority::mutex());
    running().erase(node.uuid());

  }
}

priority::node priority::start(const char * fileName, const char * params, int priority, bool purge)
{
  std::unique_lock<std::mutex> lck(priority::mutex());
  priority::node node = priority::node(fileName, params, priority, purge);
  priority::push( node );

  if(priority::actives() < priority::max()) {
    core::start(new arken::concurrent::task::priority());
  }

  return node;
}

priority::node::node()
{}

priority::node::node(const node &obj)
{
  m_uuid      = obj.m_uuid;
  m_fileName  = obj.m_fileName;
  m_params    = obj.m_params;
  m_priority  = obj.m_priority;
  m_microtime = obj.m_microtime;
  m_shared    = obj.m_shared;
  m_finished  = obj.m_finished;
  m_inspect   = obj.m_inspect;
  m_purge     = obj.m_purge;
}

priority::node::node(const char * fileName, const char * params, int priority, bool purge)
{
  m_uuid      = os::uuid();
  m_fileName  = fileName;
  m_params    = params;
  m_priority  = priority;
  m_purge     = purge;
  m_microtime = os::microtime();
  m_finished  = std::shared_ptr<std::atomic<bool>>(new std::atomic<bool>(false));
  m_inspect.
    append("arken.concurrent.task.fifo: ").
    append(m_fileName).append("#").
    append(m_params.escape());
}

bool priority::node::operator()(const priority::node &n1, const priority::node &n2)
{
  if( n1.m_priority == n2.m_priority ) {
    return n1.m_microtime > n2.m_microtime;
  }

  return n1.m_priority < n2.m_priority;
}

void priority::node::run()
{
  int rv;
  mvm::instance i = mvm::getInstance(m_purge);
  i.swap(m_shared);

  lua_State * L = i.state();
  lua_settop(L, 0);

  lua_getglobal(L,  "require");
  lua_pushstring(L, "arken.concurrent.task.priority");
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
  auto ptr = static_cast<priority::node **>(lua_newuserdata(L, sizeof(priority::node*)));
  *ptr = new priority::node(*this);
  luaL_getmetatable(L, "arken.concurrent.task.priority.node.metatable");
  lua_setmetatable(L, -2);

  json::decode(L, m_params);

  rv = lua_pcall(L, 2, 0, 0); // alterar para envio de 1
  if (rv) {
    fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
  }

  // GC
  if( m_purge ) {
    i.release();
  } else {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }

  (*m_finished.get()) = true;
}

void priority::push(const priority::node & node)
{
  priority::priority_queue().push( node );
}

priority::node priority::dequeue()
{
  std::unique_lock<std::mutex> lck(priority::mutex());
  if (priority::priority_queue().empty()) {
    return priority::node();
  }

  priority::node n = priority::priority_queue().top();
  priority::priority_queue().pop();
  running()[n.uuid()] = n.inspect();

  return n;
}

int priority::node::priority()
{
  return m_priority;
}

std::unordered_map<string, string> &priority::running()
{
  static std::unordered_map<string, string> s_running;
  return s_running;
}

string priority::inspect()
{
  std::unique_lock<std::mutex> lck(priority::mutex());

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
  tmp.append("\"queue\": ");
  tmp.append(std::to_string(priority_queue().size()));
  tmp.append("}");
  return tmp;
}

std::mutex & priority::mutex()
{
  static std::mutex s_mutex;
  return s_mutex;
}

std::atomic<uint32_t> & priority::actives()
{
  static std::atomic<uint32_t> s_actives{0};
  return s_actives;
}

std::atomic<uint32_t> & priority::max()
{
  static std::atomic<uint32_t> s_max{mvm::threads()};
  return s_max;
}

}  // namespace task
}  // namespace concurrent
}  // namespace arken
