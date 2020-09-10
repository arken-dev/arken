// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_MVM_
#define _CHARON_MVM_

#include <lua/lua.hpp>
#include <charon/base>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <condition_variable>
#include <cstring>
#include <iostream>

namespace charon
{

class instance;

class mvm {
  friend class instance;

  public:
  class data {
    friend class mvm;
    friend class instance;

    private:
    lua_State * m_State;
    int         m_version;
    int         m_gc;
    bool        m_release = false;

    public:
    data(int version);
    ~data();
    lua_State * state();
    lua_State * release();
    int version();
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

  class Concurrent {

    public:
    void run();
    bool release();
  };

  static std::atomic<double> s_uptime;
  static std::atomic<int>    s_gc;
  static std::atomic<int>    s_version;
  static std::atomic<int>    s_pool;

  static int      s_argc;
  static char  ** s_argv;
  static string   s_charonPath;
  static string   s_profilePath;
  static string   s_dispatchPath;

  private:
  static mvm::data * pop();

  static std::vector<std::thread>      * concurrent_workers;
  static std::queue<mvm::Concurrent *> * concurrent_queue;
  static std::mutex                    * concurrent_mutex;
  static std::condition_variable       * concurrent_condition;

  static uint32_t concurrent_max;
  static uint32_t concurrent_actives;
  static Concurrent * get();

  mvm() {};
  ~mvm() {};

  public:
  static void init(int argc, char ** argv);
  static void config();
  static void log(const char * value);
  static void set(std::string key, int value);
  static int  at(std::string key);
  static bool pause(std::string key);
  static void reload();
  static int  version();
  static int  pool();
  static int  gc();
  static int  clear();
  static void push(mvm::data *);
  static void back(mvm::data *);
  static double uptime();
  static charon::instance instance();
  static const char * charonPath();
  static void concurrent(Concurrent * pointer);
  static void working();

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
#endif // CHARONMVM_H
