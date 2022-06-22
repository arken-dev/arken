// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/mvm.h>
#include <arken/concurrent/base.h>
#include <arken/concurrent/core.h>
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
static std::unordered_map <std::thread::id, mvm::data *> s_mvm_map;
static std::mutex s_mvm_mutex;


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
    mvm::instance i = mvm::getInstance();
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
  const char * cext = os::cext();
  s_cpackagePath.
  append(".").append("/?.").append(cext).
  append(";").append("./clib/?.").append(cext).
  append(";").append(s_arkenPath).append("/clib/?.").append(cext);

  s_profilePath = s_arkenPath;
  s_profilePath.append("/profile.lua");

  container::init();

  mvm::config();

}

mvm::instance mvm::getInstance(bool create)
{

  if( create ) {
    return mvm::instance( new mvm::data() );
  }

  mvm::data * data = mvm::pop();

  if( mvm::s_version > data->m_version ) {
    delete data;
    data = new mvm::data();
  }

  return mvm::instance(data);
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
  return arken::concurrent::core::max();
}

void mvm::threads(uint32_t threads)
{
  arken::concurrent::core::max() = threads;
}

double mvm::uptime()
{
  return os::microtime() - mvm::s_uptime;
}

const char * mvm::path()
{
  return s_arkenPath;
}

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

/*
void arken::concurrent(concurrent::base * ptr)
{
  arken::concurrent::core::start(ptr);
}
*/

uint32_t mvm::actives()
{
  return arken::concurrent::core::actives();
}

string mvm::inspect()
{
  std::unique_lock<std::mutex> lck(arken::concurrent::core::mutex());

  int count = 0;
  string tmp("{");
  tmp.append("\"running\": [");

  for (std::pair<std::thread::id, concurrent::base *> element : concurrent::core::running()) {
    if( count > 0 ) {
      tmp.append(",");
    }
    tmp.append("\"").append(element.second->inspect()).append("\"");
    count++;
  }

  tmp.append("],");

  count = 0;
  tmp.append("\"wait\": [");
  for (std::pair<string, string> element : arken::concurrent::core::waiting()) {
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
  return arken::concurrent::core::workers().size();
}

void mvm::wait()
{

  while( true ) {
    {
      std::unique_lock<std::mutex> lck(arken::concurrent::core::mutex());
      if (arken::concurrent::core::actives() == 0 && arken::concurrent::core::queue().empty()) {
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

mvm::data * mvm::current()
{
  std::unique_lock<std::mutex> lck(s_mvm_mutex);
  return s_mvm_map.at(std::this_thread::get_id());
}

arken::mvm::Shared & mvm::shared()
{
  static Shared instance;
  return instance;
}

char * mvm::setlocale(string locale)
{
  return std::setlocale(LC_ALL, locale);
}

//-----------------------------------------------------------------------------
// DATA
//-----------------------------------------------------------------------------

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
  lua_close(m_State);
}

lua_State * mvm::data::state()
{
  return m_State;
}

void mvm::data::release()
{
  m_release = true;
}

uint32_t mvm::data::version()
{
  return m_version;
}

arken::mvm::Shared mvm::data::shared()
{
  return m_shared;
}

//-----------------------------------------------------------------------------
// INSTANCE
//-----------------------------------------------------------------------------

mvm::instance::instance(mvm::data * data)
{
  m_data = data;

  std::unique_lock<std::mutex> lck(s_mvm_mutex);
  s_mvm_map[std::this_thread::get_id()] = data;
}

mvm::instance::~instance()
{

  if( m_data->m_release ) {
    delete m_data;
  } else {
    mvm::push(m_data);
  }

  std::unique_lock<std::mutex> lck(s_mvm_mutex);
  s_mvm_map.erase(std::this_thread::get_id());
}

lua_State * mvm::instance::state()
{
  return m_data->state();
}

void mvm::instance::release()
{
  m_data->release();
}

void mvm::instance::swap(arken::mvm::Shared shared)
{
  this->m_data->m_shared = shared;
}

//-----------------------------------------------------------------------------
// SHARED
//-----------------------------------------------------------------------------

mvm::Shared::Shared()
{
  m_name  = std::shared_ptr<string>(new string);
  m_info  = std::shared_ptr<string>(new string);
  m_map   = std::shared_ptr<std::unordered_map<string, data>>(new std::unordered_map<string, data>);
  m_mutex = std::shared_ptr<std::mutex>(new std::mutex);
};

mvm::Shared::Shared(const Shared & obj)
{
  m_name  = obj.m_name;
  m_info  = obj.m_info;
  m_map   = obj.m_map;
  m_mutex = obj.m_mutex;
}

void mvm::Shared::name(string name)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  *m_name = name;
}

string mvm::Shared::name()
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  return *m_name;
}

void mvm::Shared::info(string info)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  *m_info = info;
}

string mvm::Shared::info()
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  return *m_info;
}

// NUMBER
double mvm::Shared::getNumber(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    return m_map->at(key).m_number;
  } else {
    return 0;
  }
}

void mvm::Shared::setNumber(string key, double value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  (*m_map)[key].m_number = value;
  (*m_map)[key].m_flag   = 2;
}

double mvm::Shared::increment(string key, double value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);

  if( m_map->count(key) ) {
    value += m_map->at(key).m_number;
    (*m_map)[key].m_number = value;
  } else {
    (*m_map)[key].m_number = value;
  }

  return value;
}

// STRING
string mvm::Shared::getString(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    return m_map->at(key).m_string;
  } else {
    return {};
  }
}

void mvm::Shared::setString(string key, string value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  (*m_map)[key].m_string = value;
  (*m_map)[key].m_flag   = 3;
}

string mvm::Shared::append(string key, string value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    string v = std::move(m_map->at(key).m_string);
    v.append(value);
    (*m_map)[key].m_string = std::move(v);
  } else {
    (*m_map)[key].m_string = value;
  }

  return value;
}

string mvm::Shared::prepend(string key, string value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    string v = std::move(m_map->at(key).m_string);
    v.prepend(value);
    (*m_map)[key].m_string = std::move(v);
  } else {
    (*m_map)[key].m_string = value;
  }

  return value;
}

// BOOL
bool mvm::Shared::getBool(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    return m_map->at(key).m_bool;
  } else {
    return false;
  }

  return true;
}

void mvm::Shared::setBool(string key, bool value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  (*m_map)[key].m_bool = value;
  (*m_map)[key].m_flag = 1;
}

bool mvm::Shared::toggle(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  bool result = false;
  if( m_map->count(key) ) {
    result = ! m_map->at(key).m_bool;
    (*m_map)[key].m_bool = result;
  }
  return result;
}

arken::mvm::Shared & mvm::Shared::global()
{
  static Shared instance;
  return instance;
}

void mvm::Shared::put(string key, string value)
{
  this->setString(key, value);
}

void mvm::Shared::put(string key, double value)
{
  this->setNumber(key, value);
}

void mvm::Shared::put(string key, bool value)
{
  this->setBool(key, value);
}

short mvm::Shared::flag(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    return m_map->at(key).m_flag;
  } else {
    return 0;
  }
}

mvm::Shared::data mvm::Shared::get(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    return m_map->at(key);
  } else {
    return {};
  }
}

short mvm::Shared::data::flag()
{
  return m_flag;
}

bool mvm::Shared::data::getBool()
{
  return m_bool;
}

double mvm::Shared::data::getNumber()
{
  return m_number;
}

string mvm::Shared::data::getString()
{
  return m_string;
}

} // namespace arken
