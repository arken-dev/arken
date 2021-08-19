#include <arken/concurrent/task/fifo.h>
#include <arken/os.h>
#include <cstdio>
#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/mvm>


namespace arken {
namespace concurrent {
namespace task {


std::mutex fifo::s_mutex;
std::atomic<uint32_t> fifo::s_max{mvm::threads()};
std::atomic<uint32_t> fifo::s_actives{0};

fifo::fifo()
{
  fifo::s_actives++;
}

fifo::~fifo()
{
  fifo::s_actives--;
}

std::queue<fifo::node> & fifo::fifo_queue()
{
  static std::queue<fifo::node> s_fifo_queue;
  return s_fifo_queue;
}

void fifo::run()
{
  while (true) {
    fifo::node node = dequeue();
    if( !node ) {
      break;
    }
    node.run();
  }
}

bool fifo::release()
{
  return true;
}

fifo::node fifo::start(const char * fileName, const char * params, bool purge)
{
  std::unique_lock<std::mutex> lck(fifo::s_mutex);
  fifo::node node = fifo::node(fileName, params, purge);
  fifo::push( node );

  if(fifo::s_actives < fifo::s_max) {
    mvm::concurrent( new arken::concurrent::task::fifo() );
  }

  return node;
}

fifo::node::node()
{}

fifo::node::node(const node &obj)
{
  m_uuid      = obj.m_uuid;
  m_fileName  = obj.m_fileName;
  m_params    = obj.m_params;
  m_microtime = obj.m_microtime;
  m_shared    = obj.m_shared;
  m_ref_bool  = obj.m_ref_bool;
}

fifo::node::node(const char * fileName, const char * params, bool purge)
{
  m_fileName  = fileName;
  m_params    = params;
  m_purge     = purge;
  m_uuid      = os::uuid();
  m_microtime = os::microtime();
  m_ref_bool  = std::shared_ptr<std::atomic<bool>>(new std::atomic<bool>(false));
}

void fifo::node::run()
{
  int rv;
  arken::instance i = mvm::instance(m_purge);
  lua_State * L = i.state();
  lua_settop(L, 0);

  lua_getglobal(L,  "require");
  lua_pushstring(L, "arken.concurrent.task.fifo");
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
  auto ptr = static_cast<fifo::node **>(lua_newuserdata(L, sizeof(fifo::node*)));
  *ptr = new fifo::node(*this);
  luaL_getmetatable(L, "arken.concurrent.task.fifo.node.metatable");
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

void fifo::push(const fifo::node & node)
{
  fifo::fifo_queue().push( node );
}

fifo::node fifo::dequeue()
{
  std::unique_lock<std::mutex> lck(fifo::s_mutex);
  if (fifo::fifo_queue().empty()) {
    return {};
  }

  fifo::node n = fifo::fifo_queue().front();
  fifo::fifo_queue().pop();
  return n;
}

void fifo::wait()
{
  mvm::wait();
}

string fifo::node::uuid()
{
  return m_uuid;
}

double fifo::node::microtime()
{
  return m_microtime;
}

fifo::node::operator bool() const {
  return m_microtime > 0;
}

Shared fifo::node::shared()
{
  return m_shared;
}

bool fifo::node::finished()
{
  return (*m_ref_bool.get()) == true;
}

void fifo::node::wait()
{
  while ((*m_ref_bool.get()) == false) {
    os::sleep(0.05);
  }
}

}  // namespace task
}  // namespace concurrent
}  // namespace arken
