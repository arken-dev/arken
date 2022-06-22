// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_MVM_
#define _ARKEN_MVM_

#include <lua/lua.hpp>
#include <arken/string.h>
#include <arken/concurrent/base.h>
#include <arken/concurrent/core.h>
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

namespace arken {

class mvm {
  using string = arken::string;
  using Shared = arken::concurrent::Shared;

  public:

  class data {

    using Shared = arken::concurrent::Shared;

    friend class mvm;

    private:
    lua_State * m_State;
    uint32_t    m_version;
    uint32_t    m_gc;
    bool        m_release = false;
    Shared      m_shared;

    public:
    data(uint32_t version = s_version);
    ~data();
    lua_State * state();
    void        release();
    uint32_t    version();
    Shared      shared();
  };

  class instance {
    mvm::data * m_data;

    public:
    instance(mvm::data * data);
    ~instance();
    lua_State * state();
    void release();
    void swap(arken::concurrent::Shared shared);
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
  static mvm::instance getInstance(bool create = false);
  static const char * path();
  static void wait();
  static void env(const char * value);
  static const char * env();
  static uint32_t actives();
  static string inspect();
  static size_t workers();
  static char * setlocale(string locale, string category);
  static char * setlocale(string locale);
  static mvm::data * current();
  static Shared & shared();

};

} // namespace arken
#endif // _ARKEN_MVM_
