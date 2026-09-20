// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_SECURITY_RATELIMIT_
#define _ARKEN_SECURITY_RATELIMIT_

#include <chrono>
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

  private:
  struct Record {
    unsigned hits = 0;
    std::chrono::steady_clock::time_point window_start{};
    std::chrono::steady_clock::time_point last{};
  };

  unsigned m_limit;
  std::chrono::seconds m_window;
  std::mutex m_mutex;
  std::unordered_map<std::string, Record> m_map;
};

} // namespace security
} // namespace arken

#endif
