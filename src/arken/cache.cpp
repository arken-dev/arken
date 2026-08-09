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
#include <fstream>
#include <cstdint>
#include <cstring>

namespace arken {

namespace {
  const int GC_INTERVAL_SECONDS = 5;
}

std::mutex cache::s_registryMutex;
std::unordered_map<std::string, cache::bucket *> cache::s_registry;

cache::bucket & cache::get(const char * name)
{
  std::unique_lock<std::mutex> lck(s_registryMutex);

  auto it = s_registry.find(name);
  if (it != s_registry.end()) {
    return *(it->second);
  }

  bucket * b = new bucket();
  s_registry[name] = b;
  return *b;
}

void cache::backgroundGC()
{
  while (true) {
    os::sleep(GC_INTERVAL_SECONDS);

    // copia os ponteiros dos buckets existentes sob o lock do registro, e
    // varre cada um já fora dele — gc() de um bucket não deve segurar o
    // registro (que outras threads podem precisar pra achar/criar buckets).
    std::vector<bucket *> snapshot;
    {
      std::unique_lock<std::mutex> lck(s_registryMutex);
      snapshot.reserve(s_registry.size());
      for (auto & pair : s_registry) {
        snapshot.push_back(pair.second);
      }
    }

    for (bucket * b : snapshot) {
      b->gc();
    }
  }
}

// primeira inserção em qualquer bucket agenda uma única thread compartilhada
// que varre todos os buckets existentes periodicamente, sem depender de
// nenhuma rotina externa.
void cache::ensureBackgroundGC()
{
  static std::once_flag flag;
  std::call_once(flag, []() {
    std::thread(cache::backgroundGC).detach();
  });
}

std::optional<std::string> cache::value(const char * key)
{
  return get().value(key);
}

void cache::insert(const char *key, const char * value, int expires)
{
  get().insert(key, value, expires);
}

void cache::remove(const char * key)
{
  get().remove(key);
}

double cache::size()
{
  return get().size();
}

void cache::gc()
{
  get().gc();
}

std::vector<std::string> cache::keys(const char * pattern)
{
  return get().keys(pattern);
}

void cache::maxSize(double bytes)
{
  get().maxSize(bytes);
}

double cache::maxSize()
{
  return get().maxSize();
}

bool cache::dump(const char * path)
{
  return get().dump(path);
}

bool cache::load(const char * path)
{
  return get().load(path);
}

std::vector<std::string> cache::bucketNames()
{
  std::unique_lock<std::mutex> lck(s_registryMutex);

  std::vector<std::string> result;
  result.reserve(s_registry.size());
  for (auto & pair : s_registry) {
    result.push_back(pair.first);
  }

  return result;
}

// ---------------------------------------------------------------------------
// bucket
// ---------------------------------------------------------------------------

void cache::bucket::touch(std::list<Entry>::iterator it)
{
  m_order.splice(m_order.begin(), m_order, it);
}

void cache::bucket::evict()
{
  if (m_maxBytes <= 0) {
    return;
  }

  while (m_bytes > m_maxBytes && !m_order.empty()) {
    Entry & back = m_order.back();
    m_bytes -= back.second->value().size();
    delete back.second;
    m_index.erase(back.first);
    m_order.pop_back();
  }
}

std::optional<std::string> cache::bucket::value(const char * key)
{
  return get(key);
}

std::optional<std::string> cache::bucket::get(const char * key)
{
  std::unique_lock<std::mutex> lck(m_mutex);

  auto it = m_index.find(key);
  if (it == m_index.end()) {
    return std::nullopt;
  }

  auto orderIt = it->second;
  data * d = orderIt->second;

  if (d->isExpires()) {
    m_bytes -= d->value().size();
    delete d;
    m_order.erase(orderIt);
    m_index.erase(it);
    return std::nullopt;
  }

  touch(orderIt);
  return d->value();
}

void cache::bucket::insert(const char *key, const char * value, int expires)
{
  // json::encode sempre devolve uma string C válida terminada em '\0' sem
  // byte nulo embutido, então strlen() é seguro pro caminho JSON.
  put(key, value, strlen(value), expires);
}

void cache::bucket::put(const char *key, const char * value, size_t size,
                         int expires)
{
  cache::ensureBackgroundGC();

  std::string content(value, size);

  std::unique_lock<std::mutex> lck(m_mutex);

  auto it = m_index.find(key);
  if (it != m_index.end()) {
    auto orderIt = it->second;
    data * old = orderIt->second;
    m_bytes -= old->value().size();
    delete old;
    orderIt->second = new data(content, expires);
    m_bytes += orderIt->second->value().size();
    touch(orderIt);
  } else {
    m_order.emplace_front(std::string(key), new data(content, expires));
    m_index[key] = m_order.begin();
    m_bytes += m_order.begin()->second->value().size();
  }

  evict();
}

void cache::bucket::remove(const char * key)
{
  std::unique_lock<std::mutex> lck(m_mutex);

  auto it = m_index.find(key);
  if (it != m_index.end()) {
    auto orderIt = it->second;
    data * d = orderIt->second;
    m_bytes -= d->value().size();
    delete d;
    m_order.erase(orderIt);
    m_index.erase(it);
  }
}

double cache::bucket::size()
{
  std::unique_lock<std::mutex> lck(m_mutex);
  return m_bytes;
}

void cache::bucket::maxSize(double bytes)
{
  std::unique_lock<std::mutex> lck(m_mutex);
  m_maxBytes = bytes;
  evict();
}

double cache::bucket::maxSize()
{
  std::unique_lock<std::mutex> lck(m_mutex);
  return m_maxBytes;
}

std::vector<std::string> cache::bucket::keys(const char * pattern)
{
  std::unique_lock<std::mutex> lck(m_mutex);

  std::vector<std::string> result;

  for (Entry & entry : m_order) {
    if (entry.second->isExpires()) {
      continue;
    }
    if (utils::glob::match(entry.first, pattern)) {
      result.push_back(entry.first);
    }
  }

  return result;
}

void cache::bucket::gc()
{
  std::unique_lock<std::mutex> lck(m_mutex);

  auto it = m_order.begin();
  while (it != m_order.end()) {
    if (it->second->isExpires()) {
      m_bytes -= it->second->value().size();
      delete it->second;
      m_index.erase(it->first);
      it = m_order.erase(it);
    } else {
      ++it;
    }
  }

  evict();
}

// formato: [uint64 count] seguido de "count" registros
// [int32 expiresAt][uint32 keyLen][uint32 valueLen][key bytes][value bytes]
// tamanhos explícitos em vez de qualquer delimitador/escape, então chave e
// valor podem conter bytes arbitrários (inclusive nulos) sem ambiguidade.
bool cache::bucket::dump(const char * path)
{
  std::unique_lock<std::mutex> lck(m_mutex);

  std::ofstream out(path, std::ios::binary | std::ios::trunc);
  if (!out) {
    return false;
  }

  uint64_t count = m_order.size();
  out.write(reinterpret_cast<const char *>(&count), sizeof(count));

  for (Entry & entry : m_order) {
    const std::string & key   = entry.first;
    const std::string & value = entry.second->value();
    int32_t expiresAt = entry.second->expiresAt();
    uint32_t keyLen    = (uint32_t) key.size();
    uint32_t valueLen  = (uint32_t) value.size();

    out.write(reinterpret_cast<const char *>(&expiresAt), sizeof(expiresAt));
    out.write(reinterpret_cast<const char *>(&keyLen), sizeof(keyLen));
    out.write(reinterpret_cast<const char *>(&valueLen), sizeof(valueLen));
    out.write(key.data(), keyLen);
    out.write(value.data(), valueLen);
  }

  return (bool) out;
}

// preserva o TTL restante de cada chave (não dá um TTL novo do zero); chaves
// já expiradas no momento do load() são descartadas em vez de restauradas.
// passa por insert() (não escreve m_order/m_index diretamente) pra reusar a
// mesma lógica de touch/evict/gc automático, então não segura m_mutex aqui
// — cada insert() cuida do próprio lock.
bool cache::bucket::load(const char * path)
{
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    return false;
  }

  uint64_t count = 0;
  in.read(reinterpret_cast<char *>(&count), sizeof(count));

  for (uint64_t i = 0; i < count && in; i++) {
    int32_t expiresAt = 0;
    uint32_t keyLen = 0, valueLen = 0;

    in.read(reinterpret_cast<char *>(&expiresAt), sizeof(expiresAt));
    in.read(reinterpret_cast<char *>(&keyLen), sizeof(keyLen));
    in.read(reinterpret_cast<char *>(&valueLen), sizeof(valueLen));

    std::string key(keyLen, '\0');
    in.read(&key[0], keyLen);

    std::string value(valueLen, '\0');
    in.read(&value[0], valueLen);

    if (!in) {
      break;
    }

    int expires;
    if (expiresAt <= 0) {
      expires = 0; // nunca expira
    } else {
      double remaining = expiresAt - os::microtime();
      if (remaining <= 0) {
        continue; // já expirou entre o dump() e este load(), descarta
      }
      expires = (int) remaining;
    }

    insert(key.c_str(), value.c_str(), expires);
  }

  return true;
}

// ---------------------------------------------------------------------------
// bucket::data
// ---------------------------------------------------------------------------

cache::bucket::data::data(const std::string & value, int expires)
  : m_value(value)
{
  if( expires < 0 ) {
    m_expires = os::microtime() + 60;
  } else if( expires == 0 ) {
    // expires == 0 é o sentinel para "nunca expira" (isExpires() trata
    // m_expires <= 0 como sem TTL)
    m_expires = 0;
  } else {
    m_expires = os::microtime() + expires;
  }
}

const std::string & cache::bucket::data::value()
{
  return m_value;
}

bool cache::bucket::data::isExpires()
{
  if ( m_expires > 0 ) {
    return os::microtime() >= m_expires;
  } else {
    return false;
  }
}

int cache::bucket::data::expiresAt()
{
  return m_expires;
}

} // namespace arken
