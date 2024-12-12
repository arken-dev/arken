// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/cache>
#include <arken/json>

namespace arken {

std::mutex cache::s_mutex;
std::unordered_map<std::string, cache::data *> * cache::s_cache = new std::unordered_map<std::string, cache::data *>;

const char * cache::value(const char * key)
{
  std::unique_lock<std::mutex> lck(s_mutex);

  if (s_cache->find(key) == s_cache->end()) {
    return nullptr;
  } else {
    cache::data * data = s_cache->at(key);
    if ( data->isExpires() ) {
      s_cache->erase(key);
      delete data;
      return nullptr;
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
    m_expires = 60;
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

double cache::size()
{
  double result = 0;

  std::unique_lock<std::mutex> lck(s_mutex);
  for (std::pair<std::string, cache::data *> element : *cache::s_cache) {
    result = result + strlen(element.second->value());
  }

  return result;
}

void cache::gc()
{
  std::unique_lock<std::mutex> lck(s_mutex);
  for (std::pair<std::string, cache::data *> element : *cache::s_cache) {
    if( element.second->isExpires() ) {
      std::string key = element.first;
      // TODO criar método remove sem mutex exemplo _remove
      cache::data * data = s_cache->at(key);
      delete data;
      s_cache->erase(key);
    }
  }
}

} // namespace arken
