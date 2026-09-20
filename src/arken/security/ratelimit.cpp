// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/security/ratelimit.h>
#include <thread>
#include <vector>

namespace arken {
namespace security {

namespace {
  const int GC_INTERVAL_SECONDS = 5;
}

using clock = std::chrono::steady_clock;

std::mutex RateLimit::s_registryMutex;
std::unordered_map<std::string, RateLimit *> RateLimit::s_registry;

RateLimit::RateLimit(unsigned limit, unsigned seconds)
  : m_limit(limit)
  , m_window(seconds == 0 ? 1 : seconds)
{
}

RateLimit::~RateLimit() = default;

RateLimit & RateLimit::get(const char * name, unsigned limit, unsigned seconds)
{
  std::lock_guard<std::mutex> lock(s_registryMutex);

  auto it = s_registry.find(name);
  if( it != s_registry.end() ) {
    return *(it->second);
  }

  RateLimit * rl = new RateLimit(limit, seconds);
  s_registry[name] = rl;

  ensureBackgroundGC();

  return *rl;
}

void RateLimit::backgroundGC()
{
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(GC_INTERVAL_SECONDS));

    // copia os ponteiros sob o lock do registro, e varre cada um já fora
    // dele -- gc() de uma instância não deve segurar o registro (que
    // outras threads podem precisar pra achar/criar instâncias).
    std::vector<RateLimit *> snapshot;
    {
      std::lock_guard<std::mutex> lock(s_registryMutex);
      snapshot.reserve(s_registry.size());
      for (auto & pair : s_registry) {
        snapshot.push_back(pair.second);
      }
    }

    for (RateLimit * rl : snapshot) {
      rl->gc(static_cast<unsigned>(rl->m_window.count() * 2));
    }
  }
}

void RateLimit::ensureBackgroundGC()
{
  static std::once_flag flag;
  std::call_once(flag, []() {
    std::thread(RateLimit::backgroundGC).detach();
  });
}

bool RateLimit::count(const char * ip)
{
  if( ip == nullptr || ip[0] == '\0' ) {
    return false;
  }

  const auto now = clock::now();
  std::lock_guard<std::mutex> lock(m_mutex);
  auto & r = m_map[ip];

  if( r.window_start.time_since_epoch().count() == 0 ||
      now - r.window_start >= m_window ) {
    r.hits = 0;
    r.window_start = now;
  }

  r.hits += 1;
  r.last  = now;
  return r.hits >= m_limit;
}

void RateLimit::clear(const char * ip)
{
  if( ip == nullptr ) {
    return;
  }
  std::lock_guard<std::mutex> lock(m_mutex);
  m_map.erase(ip);
}

void RateLimit::sweep(std::chrono::seconds idle)
{
  const auto now = clock::now();
  for( auto it = m_map.begin(); it != m_map.end(); ) {
    if( now - it->second.last > idle ) {
      it = m_map.erase(it);
    } else {
      ++it;
    }
  }
}

void RateLimit::gc(unsigned idle_seconds)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  sweep(std::chrono::seconds(idle_seconds));
}

size_t RateLimit::size()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_map.size();
}

} // namespace security
} // namespace arken
