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

  // instância única do processo para `name` -- registro estático em C++,
  // fora de qualquer lua_State, então é a mesma para todas as VMs do pool
  // mvm (cada lua_State tem seu próprio require()/package.loaded; sem
  // isso, cada VM do pool teria seu próprio contador e o rate limit
  // furaria dependendo de qual VM atende cada request). Criada com
  // limit/seconds na primeira chamada; chamadas seguintes com o mesmo
  // nome ignoram limit/seconds e devolvem a instância já existente.
  // Mesmo esquema de arken::cache::get(name)/cache.bucket(name) -- e,
  // como consequência, uma instância obtida por aqui nunca é destruída
  // (vive pelo tempo de vida do processo, como os buckets do cache).
  static RateLimit & get(const char * name, unsigned limit, unsigned seconds = 60);

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
  // precisa já estar segurando m_mutex (usado por gc() e pela thread de
  // faxina compartilhada).
  void sweep(std::chrono::seconds idle);

  unsigned m_limit;
  std::chrono::seconds m_window;
  std::mutex m_mutex;
  std::unordered_map<std::string, Record> m_map;

  // registro global name -> instância; nunca libera entradas (mesma
  // premissa do registro de buckets do arken::cache: vive pelo tempo de
  // vida do processo, por isso é seguro varrer via thread de background).
  static std::mutex s_registryMutex;
  static std::unordered_map<std::string, RateLimit *> s_registry;

  // primeira chamada a get() agenda uma única thread compartilhada que
  // varre todas as instâncias registradas periodicamente, descartando
  // IPs ociosos há mais de 2x a janela de cada uma -- mesmo esquema de
  // arken::cache::backgroundGC()/ensureBackgroundGC(). Sem isso, m_map só
  // cresce: um IP visto uma única vez (comum: milhares de IPs distintos
  // por dia de tráfego real) nunca seria removido, já que count() só
  // reseta a janela de um IP que volta a aparecer.
  static void backgroundGC();
  static void ensureBackgroundGC();
};

} // namespace security
} // namespace arken

#endif
