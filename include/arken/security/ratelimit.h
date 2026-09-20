// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_SECURITY_RATELIMIT_
#define _ARKEN_SECURITY_RATELIMIT_

#include <chrono>
#include <cstddef>
#include <mutex>
#include <string>
#include <unordered_map>

namespace arken {
namespace security {

// janela fixa: no primeiro hit do IP a janela abre; depois de
// `seconds` segundos os hits zeram e uma nova janela começa.
// Não é token bucket.
class RateLimit
{
  public:
  RateLimit(unsigned limit, unsigned seconds = 60);
  ~RateLimit();

  // incrementa o IP; true = estourou o limite (avisar a borda)
  bool count(const char * ip);
  void clear(const char * ip);
  void gc(unsigned idle_seconds);

  // número de IPs atualmente rastreados (introspecção/monitoramento; útil
  // também pra provar que a faxina, manual ou automática, não deixa
  // m_map crescer sem limite)
  size_t size();

  private:
  struct Record {
    unsigned hits = 0;
    std::chrono::steady_clock::time_point window_start{};
    std::chrono::steady_clock::time_point last{};
  };

  // varre m_map descartando entradas ociosas há mais de `idle`; chamador
  // precisa já estar segurando m_mutex (usado por gc() e pela faxina
  // amortizada dentro de count()).
  void sweep(std::chrono::seconds idle);

  unsigned m_limit;
  std::chrono::seconds m_window;
  std::mutex m_mutex;
  std::unordered_map<std::string, Record> m_map;
  std::chrono::steady_clock::time_point m_lastSweep{};
};

} // namespace security
} // namespace arken

#endif
