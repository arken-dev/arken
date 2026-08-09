// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/cache>
#include <arken/json>
#include <arken/utils/glob.h>
#include <vector>
#include <thread>

namespace arken {

namespace {
  const int GC_INTERVAL_SECONDS = 5;

  void backgroundGC()
  {
    while (true) {
      os::sleep(GC_INTERVAL_SECONDS);
      cache::gc();
    }
  }

  // primeira inserção agenda uma thread própria que roda gc() periodicamente
  // pelo tempo de vida do processo, sem depender de nenhuma rotina externa
  void ensureBackgroundGC()
  {
    static std::once_flag flag;
    std::call_once(flag, []() {
      std::thread(backgroundGC).detach();
    });
  }
}

std::mutex cache::s_mutex;
std::unordered_map<std::string, cache::data *> * cache::s_cache = new std::unordered_map<std::string, cache::data *>;

std::optional<std::string> cache::value(const char * key)
{
  std::unique_lock<std::mutex> lck(s_mutex);

  if (s_cache->find(key) == s_cache->end()) {
    return std::nullopt;
  } else {
    cache::data * data = s_cache->at(key);
    if ( data->isExpires() ) {
      s_cache->erase(key);
      delete data;
      return std::nullopt;
    } else {
      return data->value();
    }
  }
}

void cache::insert(const char *key, const char * value, int expires)
{
  ensureBackgroundGC();

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

cache::data::data(const std::string & value, int expires) : m_value(value)
{
  if( expires < 0 ) {
    m_expires = os::microtime() + 60;
  } else if( expires == 0 ) {
    // expires == 0 é o sentinel para "nunca expira" (isExpires() trata m_expires <= 0 como sem TTL)
    m_expires = 0;
  } else {
    m_expires = os::microtime() + expires;
  }
}

const std::string & cache::data::value()
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
    result = result + element.second->value().size();
  }

  return result;
}

std::vector<std::string> cache::keys(const char * pattern)
{
  std::unique_lock<std::mutex> lck(s_mutex);

  std::vector<std::string> result;

  for (std::pair<std::string, cache::data *> element : *cache::s_cache) {
    if ( element.second->isExpires() ) {
      continue;
    }
    if ( utils::glob::match(element.first, pattern) ) {
      result.push_back(element.first);
    }
  }

  return result;
}

void cache::gc()
{
  std::unique_lock<std::mutex> lck(s_mutex);

  std::vector<std::string> list;

  for (std::pair<std::string, cache::data *> element : *cache::s_cache) {
    if( element.second->isExpires() ) {
      std::string key = element.first;
      list.push_back(key);
    }
  }

  for( long unsigned int i=0; i < list.size(); i++ ) {
    std::string key = list.at(i);
    cache::data * data = s_cache->at(key);
    delete data;
    s_cache->erase(key);
  }

}

} // namespace arken
