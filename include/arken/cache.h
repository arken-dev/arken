// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CACHE_
#define _ARKEN_CACHE_

#include <mutex>
#include <unordered_map>
#include <arken/base>

namespace arken
{

class cache {
  public:
  static const char * value(const char * key);
  static void insert(const char *key, const char * value, int expires = -1);
  static void remove(const char * key);

  private:

  class data {
    public:
    data(const char * value, int expires);
    ~data();
    const char * value();
    bool isExpires();

    private:
    char * m_value;
    double m_expires;
  };

  static std::mutex s_mutex;
  static std::unordered_map<std::string, data *> * s_cache;

};

} // namespace arken

#endif // ARKENCACHE_H
