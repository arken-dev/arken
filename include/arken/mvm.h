// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_MVM_
#define _ARKEN_MVM_

#include <lua/lua.hpp>
#include <arken/string.h>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <condition_variable>
#include <cstring>
#include <iostream>
#include <atomic>

using arken::string;

namespace arken
{

namespace concurrent
{
  class Base {

    protected:
    std::atomic<bool> m_finished = ATOMIC_VAR_INIT(false);

    public:
    virtual void run() = 0;
    virtual bool release() = 0;
    virtual ~Base();
    bool finished();
    void finished(bool flag);
  };
}

class instance;

class mvm {

  friend class instance;

  public:

  class data {
    friend class mvm;
    friend class instance;

    private:
    lua_State * m_State;
    uint32_t    m_version;
    uint32_t    m_gc;
    bool        m_release = false;

    public:
    data(uint32_t version = s_version);
    ~data();
    lua_State * state();
    lua_State * release();
    uint32_t    version();
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


  static std::atomic<double> s_uptime;
  static std::atomic<uint32_t> s_gc;
  static std::atomic<uint32_t> s_version;
  static std::atomic<uint32_t> s_pool;

  static int      s_argc;
  static char  ** s_argv;
  static string   s_arkenPath;
  static string   s_profilePath;
  static string   s_dispatchPath;

  private:
  static mvm::data * pop();

  static std::vector<std::thread>       * concurrent_workers;
  static std::queue<concurrent::Base *> * concurrent_queue;
  static std::mutex                     * concurrent_mutex;
  static std::condition_variable        * concurrent_condition;

  static std::atomic<uint32_t> concurrent_max;
  static std::atomic<uint32_t> concurrent_actives;

  static concurrent::Base * get();

  mvm() {};
  ~mvm() {};

  public:
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
  static void push(mvm::data *);
  static void back(mvm::data *);
  static double uptime();
  static arken::instance instance();
  static const char * arkenPath();
  static void concurrent(concurrent::Base * pointer);
  static void working();
  static void wait();

};

class instance {


  mvm::data * m_data;
  public:
  instance(mvm::data * data);
  ~instance();
  lua_State * state();
  lua_State * release();

};

}
#endif // ARKENMVM_H
