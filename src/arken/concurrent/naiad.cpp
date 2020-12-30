#include <arken/concurrent/naiad.h>
#include <arken/os.h>
#include <cstdio>
#include <lua/lua.hpp>
#include <arken/mvm>

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * params);

using mvm = arken::mvm;
using namespace arken::concurrent;

std::priority_queue<naiad::node, std::vector<naiad::node *>, naiad::node> naiad::s_priority_queue;
std::mutex * naiad::s_mutex = new std::mutex;
std::atomic<int> naiad::s_max(1);
std::atomic<int> naiad::s_actives(0);

naiad::naiad()
{
  naiad::s_actives++;
}

naiad::~naiad()
{
  naiad::s_actives--;
}

void naiad::run()
{
  while (true) {
    naiad::node * n = dequeue();
    if( n == nullptr ) {
      break;
    } else {
      n->run();
      delete n;
    }
  }
}

bool naiad::release()
{
  return true;
}

naiad::node naiad::start(const char * fileName, const char * params, int priority, bool purge)
{
  std::unique_lock<std::mutex> lck(*naiad::s_mutex);
  naiad::node * node = new naiad::node(fileName, params, priority, purge);
  naiad::push( node );

  if(naiad::s_actives < naiad::s_max) {
    mvm::concurrent( new naiad() );
  }

  return naiad::node(*node);
}

naiad::node::node()
{}

naiad::node::node(const node &obj)
{
  m_uuid      = obj.m_uuid;
  m_fileName  = obj.m_fileName;
  m_params    = obj.m_params;
  m_priority  = obj.m_priority;
  m_microtime = obj.m_microtime;
}

naiad::node::node(const char * fileName, const char * params, int priority, bool purge)
{
  m_fileName  = fileName;
  m_params    = params;
  m_priority  = priority;
  m_purge     = purge;
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
}

bool naiad::node::operator()(naiad::node* n1, naiad::node* n2)
{
  if( n1->m_priority == n2->m_priority ) {
    return n1->m_microtime > n2->m_microtime;
  } else {
    return n1->m_priority < n2->m_priority;
  }
}

void naiad::node::run()
{
  int rv;
  arken::instance i = mvm::instance(m_purge);
  lua_State * L = i.state();
  lua_settop(L, 0);

  lua_getglobal(L,  "require");
  lua_pushstring(L, "arken.concurrent.naiad");
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
  naiad::node **ptr = (naiad::node **)lua_newuserdata(L, sizeof(naiad::node*));
  *ptr = new naiad::node(*this);
  luaL_getmetatable(L, "arken.concurrent.naiad.node.metatable");
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

void naiad::push(naiad::node * ptr)
{
  naiad::s_priority_queue.push( ptr );
}

naiad::node * naiad::dequeue()
{
  std::unique_lock<std::mutex> lck(*naiad::s_mutex);
  if (naiad::s_priority_queue.empty()) {
    return nullptr;
  }

  naiad::node * n = naiad::s_priority_queue.top();
  naiad::s_priority_queue.pop();
  return n;
}

void naiad::wait()
{
  mvm::wait();
}

string naiad::node::uuid()
{
  return m_uuid;
}

int naiad::node::priority()
{
  return m_priority;
}

double naiad::node::microtime()
{
  return m_microtime;
}
