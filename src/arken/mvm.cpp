// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/mvm>
#include <map>
#include <clocale>

namespace arken {

using arken::string;

int     mvm::s_argc{0};
char ** mvm::s_argv{nullptr};

std::atomic<uint32_t> mvm::s_gc(0);
std::atomic<uint32_t> mvm::s_version(0);
std::atomic<uint32_t> mvm::s_pool(0);
std::atomic<double>   mvm::s_uptime(os::microtime());


string mvm::s_arkenPath    = "";
string mvm::s_profilePath  = "";
string mvm::s_packagePath  = "";
string mvm::s_cpackagePath = "";
string mvm::s_env          = "development";

static std::mutex mtx;
static std::unordered_map <std::string, int> s_config;

void mvm::set(std::string key, int value)
{
  mtx.lock();
  s_config[key] = value;
  mtx.unlock();
}

int mvm::at(std::string key)
{
  return s_config[key];
}

bool mvm::pause(std::string key)
{
  int value = 0;
  while( true ) {
    os::sleep(1);
    value ++;
    int pause = mvm::at(key);
    if ( value >= pause ) {
      break;
    }
  }
  return false;
}


void mvm::log(const char * value)
{
  mtx.lock();
  std::cout << value << std::endl;
  mtx.unlock();
}

void mvm::config()
{

  // defaults values
  mvm::set("gc.pause",   60);
  mvm::set("gc.log",      0);
  mvm::set("pool.pause", 30);
  mvm::set("pool.size",   2);
  mvm::set("pool.log",    0);

  const char * fileName = "config/mvm.lua";
  if( os::exists(fileName) ) {
    arken::instance i = mvm::instance();
    lua_State * L = i.state();
    lua_settop(L, 0);
    lua_getglobal(L, "dofile");
    lua_pushstring(L, fileName);
    int rv = lua_pcall(L, 1, 1, 0);
    if (rv) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
      throw;
    }
  }
}

void mvm::args(int argc, char ** argv)
{
  s_argc  = argc;
  s_argv  = new char*[argc+1];
  for( int i=0; i < argc; i++ ) {
    int len = strlen(argv[i]);
    s_argv[i] = new char[len+1]();
    strncpy(s_argv[i], argv[i], len);
  }
}

void mvm::init(int argc, char ** argv)
{

  // args
  mvm::args(argc, argv);

  // env
  mvm::env( getenv("ARKEN_ENV") );

  //TODO

  const char * arkenPath = getenv("ARKEN_PATH");
  if( arkenPath ) {
    s_arkenPath = arkenPath;
  } else {
    string path = os::executablePath();
    s_arkenPath = path.prefix("bin").left(-1);
  }

  if( s_arkenPath.empty() ) {
    std::cerr << "os.executablePath() return empty"     << std::endl;
    std::cerr << "setenv ARKEN_PATH is a solution" << std::endl;
    exit(1);
  }

  s_packagePath.
    append("./?.lua;").
    append(s_arkenPath).append("/?.lua;").
    append(s_arkenPath).append("/lib/?.lua;").
    append(s_arkenPath).append("/packages/?.lua");

  //TODO capitalize windows
  const char * cext = mvm::cext();
  s_cpackagePath.
  append(".").append("/?.").append(cext).
  append(";").append("./clib/?.").append(cext).
  append(";").append(s_arkenPath).append("/clib/?.").append(cext);

  s_profilePath = s_arkenPath;
  s_profilePath.append("/profile.lua");

  container::init();

  mvm::config();

}

instance mvm::instance(bool create)
{

  if( create ) {
    return arken::instance( new mvm::data() );
  }

  mvm::data * data = mvm::pop();

  if( mvm::s_version > data->m_version ) {
    delete data;
    data = new mvm::data();
  }

  return arken::instance(data);
}

void mvm::push(mvm::data * data)
{
  container::push(data);
  s_pool ++;
}

void mvm::back(mvm::data * data)
{
  if( mvm::s_version > data->m_version ) {
    delete data;
  } else {
    container::back(data);
    s_pool ++;
  }
}

mvm::data * mvm::pop()
{
  mvm::data * data = container::pop();

  if( ! data ) {
    return new mvm::data();
  } else {
    s_pool --;
  }

  return data;
}

double mvm::reload()
{
  double init = os::microtime();
  int log = mvm::at("pool.log");

  if( log ) {
    char buffer[30];
    sprintf(buffer, "reload: pool size %i", s_pool.load(std::memory_order_relaxed));
    mvm::log(buffer);
  }

  while( true ) {
    mvm::data * data = mvm::pop();
    if( data->m_version > mvm::s_version ) {
      mvm::push(data);
      mvm::s_version++;
      break;
    } else {
      if( log ) {
        mvm::log("mvm create and delete");
      }
      mvm::back(new mvm::data(mvm::s_version + 1));
      delete data;
    }
  }

  return os::microtime() - init;
}

uint32_t mvm::version()
{
  return mvm::s_version;
}

uint32_t mvm::pool()
{
  return s_pool;
}

uint32_t mvm::gc()
{
  uint32_t i = 0;
  mvm::data * data;

  mvm::s_gc ++;
  data = mvm::pop();

  while( data->m_gc != mvm::s_gc ) {
    lua_gc(data->state(), LUA_GCCOLLECT, 0);
    data->m_gc = mvm::s_gc;

    mvm::back(data);
    i++;

    data = mvm::pop();
  }

  mvm::back(data);

  return i;
}

uint32_t mvm::clear()
{
  int log    = mvm::at("pool.log");
  int result = 0;
  mvm::s_version++;
  while( !container::empty() ) {
    result++;
    mvm::data * data = container::pop();
    s_pool--;
    if( log ) {
      mvm::log("mvm delete");
    }
    delete data;
  }
  return result;
}

uint32_t mvm::threads()
{
  return mvm::core::max();
}

void mvm::threads(uint32_t threads)
{
  mvm::core::max() = threads;
}

double mvm::uptime()
{
  return os::microtime() - mvm::s_uptime;
}

const char *  mvm::path()
{
  return s_arkenPath;
}

mvm::data::data(uint32_t version)
{

  int rv;
  m_version = version;
  m_gc      = s_gc;
  m_State   = luaL_newstate();

  luaL_openlibs(m_State);

  lua_getglobal(m_State, "package");
  lua_pushstring(m_State, s_packagePath);
  lua_setfield(m_State, -2, "path");
  lua_pushstring(m_State, s_cpackagePath);
  lua_setfield(m_State, -2, "cpath");

  lua_pop(m_State, 1);

  int top, i;

  lua_settop(m_State, 0);
  lua_newtable(m_State);
  top = lua_gettop(m_State);
  for(i=1; i < s_argc; i++) {
    lua_pushinteger(m_State, i-1);
    lua_pushstring(m_State, s_argv[i]);
    lua_settable(m_State, top);
  }
  lua_setglobal(m_State, "arg");

  //---------------------------------------------
  rv = luaL_loadfile(m_State, s_profilePath);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  rv = lua_pcall(m_State, 0, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  int log = mvm::at("pool.log");
  if( log ) {
    mvm::log("mvm create");
  }

}

mvm::data::~data()
{
  if( m_release == false ) {
    lua_close(m_State);
  }
}

lua_State * mvm::data::state()
{
  return m_State;
}

lua_State * mvm::data::release()
{
  m_release = true;
  return m_State;
}

uint32_t mvm::data::version()
{
  return m_version;
}

arken::instance::instance(mvm::data * data)
{
  m_data = data;
}

arken::instance::~instance()
{
  if( m_data->m_release ) {
    delete m_data;
  } else {
    mvm::push(m_data);
  }
}

lua_State * instance::state()
{
  return m_data->state();
}

lua_State * instance::release()
{
  return m_data->release();
}

concurrent::base::base() : m_uuid{""}, m_microtime{0} {
}

concurrent::base::~base() = default;

bool concurrent::base::finished()
{
  return (*m_finished.get());
}

bool concurrent::base::purge()
{
  return m_purge;
}

concurrent::base::operator bool() const {
  return m_microtime > 0;
}

void concurrent::base::wait()
{
  while ((*m_finished.get()) == false) {
    os::sleep(0.05);
  }
}

double concurrent::base::microtime()
{
  return m_microtime;
}


arken::concurrent::Shared concurrent::base::shared()
{
  return m_shared;
}

bool concurrent::base::release()
{
  return true;
}

string concurrent::base::uuid()
{
  return m_uuid;
}

void concurrent::base::finished(bool flag)
{
  (*m_finished.get()) = flag;
}

string concurrent::base::inspect()
{
  return m_inspect;
}

//-----------------------------------------------------------------------------
// ENV
//-----------------------------------------------------------------------------

void mvm::env(const char * env)
{
  if( env != nullptr ) {
    s_env = env;
  }
}

const char * mvm::env()
{
  return s_env;
}

//-----------------------------------------------------------------------------
// CEXT
//-----------------------------------------------------------------------------

const char * mvm::cext()
{
  static std::map<const string, const string> s_cext {
    {"linux",   "so"},
    {"freebsd", "so"},
    {"netbsd",  "so"},
    {"windows", "dll"},
    {"macos",   "dylib"},
  };
  return s_cext[os::name()];
}

void mvm::concurrent(concurrent::base * ptr)
{
  mvm::core::start(ptr);
}

uint32_t mvm::actives()
{
  return mvm::core::actives();
}

string mvm::inspect()
{
  std::unique_lock<std::mutex> lck(mvm::core::mutex());

  int count = 0;
  string tmp("{");
  tmp.append("\"running\": [");

  for (std::pair<string, string> element : mvm::core::running()) {
    if( count > 0 ) {
      tmp.append(",");
    }
    tmp.append("\"").append(element.second).append("\"");
    count++;
  }

  tmp.append("],");

  count = 0;
  tmp.append("\"wait\": [");
  for (std::pair<string, string> element : mvm::core::waiting()) {
    if( count > 0 ) {
      tmp.append(",");
    }
    tmp.append("\"").append(element.second).append("\"");
    count++;
  }
  tmp.append("]}");

  return tmp;
}

size_t mvm::workers()
{
  return mvm::core::workers().size();
}

void mvm::wait()
{

  while( true ) {
    {
      std::unique_lock<std::mutex> lck(mvm::core::mutex());
      if (mvm::core::actives() == 0 && mvm::core::queue().empty()) {
        break;
      }
    }
    // TODO improved whithout sleep
    os::sleep(0.05);
  }

}

char * mvm::setlocale(string locale, string category)
{
  int value;
  if( category.equals("all") ) {
    value = LC_ALL;
  } else if ( category.equals("collate") ) {
    value = LC_COLLATE;
  } else if ( category.equals("ctype") ) {
    value = LC_CTYPE;
  } else if ( category.equals("monetary") ) {
    value = LC_MONETARY;
  } else if ( category.equals("numeric") ) {
    value = LC_NUMERIC;
  } else if ( category.equals("time") ) {
    value = LC_TIME;
  } else {
    value = LC_ALL;
  }
  return std::setlocale(value, locale);
}

char * mvm::setlocale(string locale)
{
  return std::setlocale(LC_ALL, locale);
}

//-----------------------------------------------------------------------------
// MVM::CORE
//-----------------------------------------------------------------------------

mvm::core::core(uint32_t max)
{
  m_max = max;
}

mvm::core::~core()
{
  for( size_t i=0; i< workers().size(); i++ ) {
    workers().at(i).detach();
  }
}

mvm::core & mvm::core::instance()
{
  static mvm::core core(os::cores());
  return core;
}

std::queue<concurrent::base *> & mvm::core::queue()
{
  return instance().m_queue;
}

std::mutex& mvm::core::mutex()
{
  return instance().m_mutex;
}

std::vector<std::thread>& mvm::core::workers()
{
  return instance().m_workers;
}

std::condition_variable  & mvm::core::condition()
{
  return instance().m_condition;
}

std::atomic<uint32_t>  & mvm::core::actives()
{
  return instance().m_actives;
}

std::atomic<uint32_t>  & mvm::core::max()
{
  return instance().m_max;
}

std::unordered_map<string, string> & mvm::core::running()
{
  return instance().m_running;
}

std::unordered_map<string, string> & mvm::core::waiting()
{
  return instance().m_waiting;
}

void mvm::core::working()
{

  while( true ) {
    concurrent::base * ptr = get();

    ptr->run();
    ptr->finished(true);

    std::unique_lock<std::mutex> lck(mutex());
    actives()--;
    running().erase(ptr->uuid());

    if( ptr->release() ) {
      delete ptr;
    }
  } // while

} // mvm::core::working


void mvm::core::start(concurrent::base * ptr)
{
  std::unique_lock<std::mutex> lck(mutex());

  if( workers().size() < max() && (workers().size() - actives()) == 0 ) {
    workers().push_back(std::thread(working));
  }

  queue().push(ptr);
  condition().notify_one();
  actives()++;
  waiting()[ptr->uuid()] = ptr->inspect();
}

concurrent::base * mvm::core::get()
{
  std::unique_lock<std::mutex> lck(mutex());
  condition().wait(lck, []{ return ! queue().empty(); });
  concurrent::base * ptr = queue().front();
  queue().pop();
  waiting().erase(ptr->uuid());
  running()[ptr->uuid()] = ptr->inspect();

  return ptr;
}

} // namespace arken
