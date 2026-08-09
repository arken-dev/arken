// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CACHE_
#define _ARKEN_CACHE_

#include <mutex>
#include <unordered_map>
#include <list>
#include <optional>
#include <string>
#include <vector>
#include <arken/base>

namespace arken
{

class cache {
  public:
  static std::optional<std::string> value(const char * key);
  static void insert(const char *key, const char * value, int expires = -1);
  static void remove(const char * key);
  static double size();
  static void   gc();
  static std::vector<std::string> keys(const char * pattern = "*");

  // limite (em bytes) de memória ocupada pelos valores armazenados; 0 (default)
  // significa sem limite. Quando o total ultrapassa o limite, as chaves menos
  // recentemente usadas (LRU) são despejadas até voltar a caber, independente
  // de TTL — inclusive entradas com expires=0 ("nunca expira" por tempo).
  static void   maxSize(double bytes);
  static double maxSize();

  private:

  class data {
    public:
    data(const std::string & value, int expires);
    ~data() = default;
    const std::string & value();
    bool isExpires();

    private:
    std::string m_value;
    int         m_expires;
  };

  // s_order mantém as chaves ordenadas por uso: início = mais recentemente
  // usada (MRU), fim = menos recentemente usada (LRU). s_index dá acesso O(1)
  // ao nó de uma chave em s_order, para mover pro início (touch) sem
  // percorrer a lista.
  using Entry = std::pair<std::string, data *>;

  static void touch(std::list<Entry>::iterator it);
  static void evict();

  static std::mutex s_mutex;
  static std::list<Entry> s_order;
  static std::unordered_map<std::string, std::list<Entry>::iterator> s_index;
  static double s_bytes;
  static double s_maxBytes;

};

} // namespace arken

#endif // ARKENCACHE_H
