// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/security/ratelimit.h>

namespace arken {
namespace security {

using clock = std::chrono::steady_clock;

RateLimit::RateLimit(unsigned limit, unsigned seconds)
  : m_limit(limit)
  , m_window(seconds == 0 ? 1 : seconds)
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

  // faxina amortizada: sem isso, m_map só cresce -- um IP que aparece uma
  // única vez nunca é removido sozinho (count() só reseta a janela de um
  // IP que volta a aparecer). Peça carona no tráfego normal em vez de
  // abrir uma thread própria: uma RateLimit é destruída pelo app (__gc do
  // binding chama delete), e uma thread detached com `this` capturado
  // ficaria com ponteiro pendurado assim que a instância for liberada.
  if( m_lastSweep.time_since_epoch().count() == 0 ||
      now - m_lastSweep >= m_window * 2 ) {
    sweep(m_window * 2);
    m_lastSweep = now;
  }

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
