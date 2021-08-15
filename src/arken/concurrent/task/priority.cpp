#include <arken/concurrent/task/priority.h>
#include <arken/os.h>
#include <cstdio>
#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/mvm>


namespace arken {
namespace concurrent {
namespace task {

std::mutex priority::s_mutex;
std::atomic<uint32_t> priority::s_max{mvm::threads()};
std::atomic<uint32_t> priority::s_actives{0};

priority::priority()
{
  priority::s_actives++;
}

priority::~priority()
{
  priority::s_actives--;
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
    node.run();
  }
}

bool priority::release()
{
  return true;
}

priority::node priority::start(const char * fileName, const char * params, int priority, bool purge)
{
  std::unique_lock<std::mutex> lck(priority::s_mutex);
  priority::node node = priority::node(fileName, params, priority, purge);
  priority::push( node );

  if(priority::s_actives < priority::s_max) {
    mvm::concurrent( new arken::concurrent::task::priority() );
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
  m_ref_bool  = obj.m_ref_bool;
}

priority::node::node(const char * fileName, const char * params, int priority, bool purge)
{
  m_fileName  = fileName;
  m_params    = params;
  m_priority  = priority;
  m_purge     = purge;
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
  m_ref_bool  = std::shared_ptr<std::atomic<bool>>(new std::atomic<bool>(false));
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
  arken::instance i = mvm::instance(m_purge);
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
}

void priority::push(const priority::node & node)
{
  priority::priority_queue().push( node );
}

priority::node priority::dequeue()
{
  std::unique_lock<std::mutex> lck(priority::s_mutex);
  if (priority::priority_queue().empty()) {
    return priority::node();
  }

  priority::node n = priority::priority_queue().top();
  priority::priority_queue().pop();
  return n;
}

void priority::wait()
{
  mvm::wait();
}

string priority::node::uuid()
{
  return m_uuid;
}

int priority::node::priority()
{
  return m_priority;
}

double priority::node::microtime()
{
  return m_microtime;
}

priority::node::operator bool() const {
  return m_microtime > 0;
}

Shared priority::node::shared()
{
  return m_shared;
}

bool priority::node::finished()
{
  return (*m_ref_bool.get()) == true;
}

void priority::node::wait()
{
  while ((*m_ref_bool.get()) == false) {
    os::sleep(0.05);
  }
}

}  // namespace task
}  // namespace concurrent
}  // namespace arken
