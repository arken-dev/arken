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
  // API estática opera implicitamente no bucket "default" (mesmo bucket
  // devolvido por get("default")) — comportamento idêntico ao de antes dos
  // buckets existirem.
  static std::optional<std::string> value(const char * key);
  static void insert(const char *key, const char * value, int expires = -1);
  static void remove(const char * key);
  static void removeAll(const char * pattern);
  static double size();
  static void   gc();
  static std::vector<std::string> keys(const char * pattern = "*");
  static void   maxSize(double bytes);
  static double maxSize();
  static bool   dump(const char * path);
  static bool   load(const char * path);

  // bucket é um cache independente: mutex, lista LRU, índice e maxSize
  // próprios, sem interferir em nenhum outro bucket. Serve pra separar
  // usos com padrões de acesso e orçamento de memória diferentes (ex.:
  // sessão vs. fragmentos de HTML) sem que um afete o outro.
  class bucket {
    public:
    std::optional<std::string> value(const char * key);
    void insert(const char *key, const char * value, int expires = -1);

    // caminho raw, sem passar por arken::json (usado pelo binding de
    // insert/value pra aceitar qualquer valor Lua) — string->string puro e
    // binário-seguro (tamanho explícito, não depende de strlen). Mais
    // rápido que insert/value pra quem só precisa guardar/ler strings.
    void put(const char *key, const char * value, size_t size,
             int expires = -1);
    std::optional<std::string> get(const char * key);

    void remove(const char * key);

    // remove todas as chaves não expiradas que casam com pattern (mesmo
    // glob de keys()). Sem default: pattern precisa ser explícito, pra não
    // virar um "apaga tudo" silencioso por argumento esquecido.
    void removeAll(const char * pattern);

    double size();
    void   gc();
    std::vector<std::string> keys(const char * pattern = "*");

    // limite (em bytes) de memória ocupada pelos valores deste bucket; 0
    // (default) significa sem limite. Quando o total ultrapassa o limite, as
    // chaves menos recentemente usadas (LRU) são despejadas até voltar a
    // caber, independente de TTL — inclusive entradas com expires=0 ("nunca
    // expira" por tempo, não imune a pressão de espaço).
    void   maxSize(double bytes);
    double maxSize();

    // grava/restaura este bucket num arquivo binário próprio. O TTL restante
    // de cada chave é preservado (uma chave com 3h pra expirar continua com
    // ~3h depois do load); chaves já expiradas no momento do load() são
    // descartadas em vez de restauradas. Devolve false se o arquivo não pôde
    // ser aberto.
    bool dump(const char * path);
    bool load(const char * path);

    private:

    class data {
      public:
      data(const std::string & value, int expires);
      ~data() = default;
      const std::string & value();
      bool isExpires();
      int  expiresAt();

      private:
      std::string m_value;
      int         m_expires;
    };

    // m_order mantém as chaves ordenadas por uso: início = mais recentemente
    // usada (MRU), fim = menos recentemente usada (LRU). m_index dá acesso
    // O(1) ao nó de uma chave em m_order, para mover pro início (touch) sem
    // percorrer a lista.
    using Entry = std::pair<std::string, data *>;

    void touch(std::list<Entry>::iterator it);
    void evict();

    std::mutex m_mutex;
    std::list<Entry> m_order;
    std::unordered_map<std::string, std::list<Entry>::iterator> m_index;
    double m_bytes    = 0;
    double m_maxBytes = 0;
  };

  // acessa (criando na primeira vez) o bucket com esse nome. "default" é o
  // mesmo bucket usado pelos métodos estáticos acima.
  static bucket & get(const char * name = "default");

  // nomes de todos os buckets já criados (por acesso via get()/insert() em
  // algum momento da vida do processo) — mesmo tipo de retorno de keys(),
  // por consistência dentro do módulo.
  static std::vector<std::string> bucketNames();

  private:
  static void backgroundGC();
  static void ensureBackgroundGC();

  static std::mutex s_registryMutex;
  static std::unordered_map<std::string, bucket *> s_registry;

};

} // namespace arken

#endif // ARKENCACHE_H
