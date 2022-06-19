// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_MVM_
#define _ARKEN_MVM_

#include <lua/lua.hpp>
#include <arken/string.h>
#include <arken/concurrent/shared.h>
#include <arken/os.h>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <condition_variable>
#include <cstring>
#include <iostream>
#include <atomic>

namespace arken
{

namespace concurrent
{
  class base {
    using Shared = arken::concurrent::Shared;
    using string = arken::string;

    protected:
    string m_uuid{os::uuid()};
    string m_params;
    string m_fileName;
    string m_inspect;
    double m_microtime{os::microtime()};
    bool m_purge{false};
    std::shared_ptr<std::atomic<bool>> m_finished{new std::atomic<bool>(false)};
    Shared m_shared;

    public:
    virtual void run() /*= 0*/;
    base();
    virtual ~base();
    bool   finished();
    virtual bool release();
    void   finished(bool flag);
    string inspect();
    string uuid();
    bool purge();
    Shared shared();
    double microtime();
    void wait();
    operator bool() const;
    static void swap(concurrent::base * source, concurrent::base * destination);
  };

} // namespace concurrent

class instance;

class mvm {
  using string = arken::string;
  using Shared = arken::concurrent::Shared;

  friend class instance;

  public:

  class data {

    using Shared = arken::concurrent::Shared;

    friend class mvm;
    friend class instance;

    private:
    lua_State * m_State;
    uint32_t    m_version;
    uint32_t    m_gc;
    bool        m_release = false;
    Shared      m_shared;


    public:
    data(uint32_t version = s_version);
    data(data & obj);
    ~data();
    lua_State * state();
    lua_State * release();
    uint32_t    version();
    Shared      shared();
  };

  class core
  {
    using string = arken::string;

    std::vector<std::thread>       m_workers;
    std::queue<concurrent::base *> m_queue;
    std::mutex                     m_mutex;
    std::condition_variable        m_condition;
    std::atomic<uint32_t>          m_max;
    std::atomic<uint32_t>          m_actives;
    std::unordered_map<string, string> m_waiting;
    std::unordered_map<std::thread::id, concurrent::base *> m_running;

    core(uint32_t max);
    ~core();
    static core & instance();
    static void working();
    public:
    static void start(concurrent::base * pointer);
    static concurrent::base * get();
    static std::queue<concurrent::base *> & queue();
    static std::mutex               & mutex();
    static std::vector<std::thread> & workers();
    static std::condition_variable  & condition();
    static std::atomic<uint32_t>    & max();
    static std::atomic<uint32_t>    & actives();
    static std::unordered_map<string, string> & waiting();
    static std::unordered_map<std::thread::id, concurrent::base *> & running();

  };

  class container {

    friend class mvm;

    private:
    static void init();
    static void back(mvm::data *);
    static mvm::data * pop();
    static bool empty();

    public:
    static void push(mvm::data *);
  };


  static std::atomic<double>   s_uptime;
  static std::atomic<uint32_t> s_gc;
  static std::atomic<uint32_t> s_version;
  static std::atomic<uint32_t> s_pool;

  static int      s_argc;
  static char  ** s_argv;
  static string   s_arkenPath;
  static string   s_profilePath;
  static string   s_packagePath;
  static string   s_cpackagePath;
  static string   s_env;

  private:
  static mvm::data * pop();


  mvm() {};
  ~mvm() {};

  public:
  static void args(int argc, char ** argv);
  static void init(int argc, char ** argv);
  static void config();
  static void log(const char * value);
  static void set(std::string key, int value);
  static int  at(std::string key);
  static bool pause(std::string key);
  static double reload();
  static uint32_t version();
  static uint32_t pool();
  static uint32_t gc();
  static uint32_t clear();
  static uint32_t threads();
  static void threads(uint32_t threads);
  static void push(mvm::data *);
  static void back(mvm::data *);
  static double uptime();
  static arken::instance instance(bool create = false);
  static const char * path();
  static void concurrent(concurrent::base * pointer);
  static void wait();
  static void env(const char * value);
  static const char * env();
  static uint32_t actives();
  static string inspect();
  static size_t workers();
  static char * setlocale(string locale, string category);
  static char * setlocale(string locale);
  static arken::mvm::data * current();
  static Shared & shared();


};

class instance {


  mvm::data * m_data;
  public:
  instance(mvm::data * data);
  ~instance();
  lua_State * state();
  lua_State * release();
  void swap(arken::concurrent::Shared shared);
};

} // namespace arken
#endif // _ARKEN_MVM_
