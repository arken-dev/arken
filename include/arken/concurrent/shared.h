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
  using string = arken::string;

  private:

  std::shared_ptr<std::unordered_map<string, string>> m_mapString;
  std::shared_ptr<std::unordered_map<string, double>> m_mapNumber;
  std::shared_ptr<std::unordered_map<string, bool>>   m_mapBool;
  std::shared_ptr<std::mutex> m_mutex;

  public:

  // NUMBER
  double getNumber(string key);
  void   setNumber(string key, double value);
  double increment(string key, double value);

  // STRING
  string getString(string key);
  void   setString(string key, string value);
  string append(string key, string value);

  // BOOL
  bool getBool(string key);
  void setBool(string key, bool value);
  bool toggle(string key);

  Shared();
  Shared(const Shared & obj);

};

} // namespace concurrent
} // namespace arken

#endif