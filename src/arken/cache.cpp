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
std::list<cache::Entry> cache::s_order;
std::unordered_map<std::string, std::list<cache::Entry>::iterator> cache::s_index;
double cache::s_bytes    = 0;
double cache::s_maxBytes = 0;

// move o nó pro início de s_order (MRU). splice não invalida iteradores, então
// o iterador guardado em s_index continua válido depois da chamada.
void cache::touch(std::list<Entry>::iterator it)
{
  s_order.splice(s_order.begin(), s_order, it);
}

// despeja do fim de s_order (LRU) até caber no limite configurado. chamador
// já precisa estar segurando s_mutex. maxSize() <= 0 (default) é "sem
// limite" e não faz nada aqui, seguindo a mesma convenção de expires == 0.
void cache::evict()
{
  if (s_maxBytes <= 0) {
    return;
  }

  while (s_bytes > s_maxBytes && !s_order.empty()) {
    Entry & back = s_order.back();
    s_bytes -= back.second->value().size();
    delete back.second;
    s_index.erase(back.first);
    s_order.pop_back();
  }
}

std::optional<std::string> cache::value(const char * key)
{
  std::unique_lock<std::mutex> lck(s_mutex);

  auto it = s_index.find(key);
  if (it == s_index.end()) {
    return std::nullopt;
  }

  auto orderIt = it->second;
  cache::data * data = orderIt->second;

  if (data->isExpires()) {
    s_bytes -= data->value().size();
    delete data;
    s_order.erase(orderIt);
    s_index.erase(it);
    return std::nullopt;
  }

  touch(orderIt);
  return data->value();
}

void cache::insert(const char *key, const char * value, int expires)
{
  ensureBackgroundGC();

  std::unique_lock<std::mutex> lck(s_mutex);

  auto it = s_index.find(key);
  if (it != s_index.end()) {
    auto orderIt = it->second;
    cache::data * old = orderIt->second;
    s_bytes -= old->value().size();
    delete old;
    orderIt->second = new data(value, expires);
    s_bytes += orderIt->second->value().size();
    touch(orderIt);
  } else {
    s_order.emplace_front(std::string(key), new data(value, expires));
    s_index[key] = s_order.begin();
    s_bytes += s_order.begin()->second->value().size();
  }

  evict();
}

void cache::remove(const char * key)
{
  std::unique_lock<std::mutex> lck(s_mutex);

  auto it = s_index.find(key);
  if (it != s_index.end()) {
    auto orderIt = it->second;
    cache::data * data = orderIt->second;
    s_bytes -= data->value().size();
    delete data;
    s_order.erase(orderIt);
    s_index.erase(it);
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
  std::unique_lock<std::mutex> lck(s_mutex);
  return s_bytes;
}

void cache::maxSize(double bytes)
{
  std::unique_lock<std::mutex> lck(s_mutex);
  s_maxBytes = bytes;
  evict();
}

double cache::maxSize()
{
  std::unique_lock<std::mutex> lck(s_mutex);
  return s_maxBytes;
}

std::vector<std::string> cache::keys(const char * pattern)
{
  std::unique_lock<std::mutex> lck(s_mutex);

  std::vector<std::string> result;

  for (Entry & entry : s_order) {
    if ( entry.second->isExpires() ) {
      continue;
    }
    if ( utils::glob::match(entry.first, pattern) ) {
      result.push_back(entry.first);
    }
  }

  return result;
}

void cache::gc()
{
  std::unique_lock<std::mutex> lck(s_mutex);

  auto it = s_order.begin();
  while (it != s_order.end()) {
    if ( it->second->isExpires() ) {
      s_bytes -= it->second->value().size();
      delete it->second;
      s_index.erase(it->first);
      it = s_order.erase(it);
    } else {
      ++it;
    }
  }

  evict();
}

} // namespace arken
