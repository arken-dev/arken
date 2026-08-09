// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CACHE_
#define _ARKEN_CACHE_

#include <mutex>
#include <unordered_map>
#include <optional>
#include <string>
#include <vector>
#include <arken/base>

namespace arken
{

class cache {
  public:
  static std::optional<std::string> value(const char * key);
  static void insert(const char *key, const char * value, int expires = -1);
  static void remove(const char * key);
  static double size();
  static void   gc();
  static std::vector<std::string> keys(const char * pattern = "*");

  private:

  class data {
    public:
    data(const std::string & value, int expires);
    ~data() = default;
    const std::string & value();
    bool isExpires();

    private:
    std::string m_value;
    int         m_expires;
  };

  static std::mutex s_mutex;
  static std::unordered_map<std::string, data *> * s_cache;

};

} // namespace arken

#endif // ARKENCACHE_H
