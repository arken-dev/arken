// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_MVM_
#define _ARKEN_MVM_

#include <lua/lua.hpp>
#include <arken/base>
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
#include <memory>

namespace arken {

class mvm {

  public:

  //---------------------------------------------------------------------------
  // SHARED
  //---------------------------------------------------------------------------

  class Shared
  {

    public:

    class data
    {
      friend class Shared;
      /**
       * 0 = undefined
       * 1 = bool
       * 2 = number
       * 3 = string
       */
      short  m_flag;
      bool   m_bool;
      double m_number;
      string m_string;

      public:
      short  flag();
      bool   getBool();
      double getNumber();
      string getString();
    };

    private:

    std::shared_ptr<string> m_name;
    std::shared_ptr<string> m_info;
    std::shared_ptr<std::unordered_map<string, data>> m_map;
    std::shared_ptr<std::mutex> m_mutex;

    public:

    // name
    void name(string name);
    string name();

    // info
    void info(string info);
    string info();

    // flag
    short flag(string key);
    data get(string key);

    // NUMBER
    double getNumber(string key);
    void   setNumber(string key, double value);
    double increment(string key, double value);

    // STRING
    string getString(string key);
    void   setString(string key, string value);
    string append(string key, string value);
    string prepend(string key, string value);

    // BOOL
    bool getBool(string key);
    void setBool(string key, bool value);
    bool toggle(string key);

    // PUT METHODS
    void put(string key, string value);
    void put(string key, double value);
    void put(string key, bool   value);

    Shared();
    Shared(const Shared & obj);
    static Shared & global();
  };

  //---------------------------------------------------------------------------
  // DATA
  //---------------------------------------------------------------------------

  class data {

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
    string      inspect();
  };

  //---------------------------------------------------------------------------
  // INSTANCE
  //---------------------------------------------------------------------------

  class instance {
    mvm::data * m_data;

    public:
    instance(mvm::data * data);
    ~instance();
    lua_State * state();
    void release();
    void swap(Shared shared);
  };

  //---------------------------------------------------------------------------
  // CONTAINER
  //---------------------------------------------------------------------------

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

  //---------------------------------------------------------------------------
  // MVM - attributes and methods
  //---------------------------------------------------------------------------

  static std::atomic<double>   s_uptime;
  static std::atomic<uint32_t> s_gc;
  static std::atomic<uint32_t> s_version;
  static std::atomic<uint32_t> s_pool;
  static std::atomic<bool>     s_shutdown;

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
  static void push(mvm::data *);
  static void back(mvm::data *);
  static double uptime();
  static mvm::instance getInstance(bool create = false);
  static const char * path();
  static void wait();
  static void shutdown();
  static bool running();
  static void env(const char * value);
  static const char * env();
  static string inspect();
  static char * setlocale(string locale, string category);
  static char * setlocale(string locale);
  static mvm::data * current();
  static Shared & shared();

};

} // namespace arken
#endif // _ARKEN_MVM_
