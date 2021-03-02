// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/cache>

namespace arken {

std::mutex cache::s_mutex;
std::unordered_map<std::string, cache::data *> * cache::s_cache = new std::unordered_map<std::string, cache::data *>;

const char * cache::value(const char * key)
{
  std::unique_lock<std::mutex> lck(s_mutex);

  if (s_cache->find(key) == s_cache->end()) {
    return NULL;
  } else {
    cache::data * data = s_cache->at(key);
    if ( data->isExpires() ) {
      s_cache->erase(key);
      delete data;
      return NULL;
    } else {
      return data->value();
    }
  }
}

void cache::insert(const char *key, const char * value, int expires)
{
  std::unique_lock<std::mutex> lck(s_mutex);

  if ( s_cache->count(key) ) {
    cache::data * data = s_cache->at(key);
    delete data;
  }

  (*s_cache)[key] = new data(value, expires);
}

void cache::remove(const char * key)
{
  std::unique_lock<std::mutex> lck(s_mutex);

  if ( s_cache->count(key) ) {
    cache::data * data = s_cache->at(key);
    delete data;
    s_cache->erase(key);
  }

}

cache::data::data(const char * value, int expires)
{

  int size = strlen(value);
  m_value  = new char[size + 1];
  strncpy(m_value, value, size);
  m_value[size] = '\0';

  if( expires < 0 ) {
    m_expires = -1;
  } else {
    m_expires = os::microtime() + expires;
  }
}

cache::data::~data()
{
  delete[] m_value;
}

const char * cache::data::value()
{
  return m_value;
}

bool cache::data::isExpires()
{
  if ( m_expires > 0 ) {
    return os::microtime() >= m_expires;
  } else {
    return false;
  }
}

} // namespace arken
