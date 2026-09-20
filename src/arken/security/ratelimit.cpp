// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/security/ratelimit.h>

namespace arken {
namespace security {

using clock = std::chrono::steady_clock;

RateLimit::RateLimit(unsigned limit, unsigned minutes)
  : m_limit(limit)
  , m_window(static_cast<long>(minutes == 0 ? 1 : minutes) * 60)
{
}

RateLimit::~RateLimit() = default;

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

void RateLimit::gc(unsigned idle_seconds)
{
  const auto now  = clock::now();
  const auto idle = std::chrono::seconds(idle_seconds);
  std::lock_guard<std::mutex> lock(m_mutex);
  for( auto it = m_map.begin(); it != m_map.end(); ) {
    if( now - it->second.last > idle ) {
      it = m_map.erase(it);
    } else {
      ++it;
    }
  }
}

} // namespace security
} // namespace arken
