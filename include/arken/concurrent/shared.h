// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_SHARED_
#define _ARKEN_CONCURRENT_SHARED_

#include <mutex>
#include <memory>
#include <unordered_map>
#include <arken/string.h>

namespace arken {
namespace concurrent {

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

  using string = arken::string;

  private:

  std::shared_ptr<string> m_info;
  std::shared_ptr<std::unordered_map<string, data>> m_map;
  std::shared_ptr<std::mutex> m_mutex;

  public:

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

} // namespace concurrent
} // namespace arken

#endif
