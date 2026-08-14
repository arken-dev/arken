// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_MAILBOX_
#define _ARKEN_CONCURRENT_MAILBOX_

#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <optional>
#include <arken/named_ptr.h>

namespace arken {
namespace concurrent {

  // Fila FIFO nomeada de mensagens, com produtores não bloqueantes e
  // consumidores bloqueantes (receive) ou não bloqueantes (fetch).
  //
  // Duas instâncias de Mailbox abertas com o mesmo nome compartilham o
  // mesmo resource via named_ptr — mesmo padrão usado por
  // arken::concurrent::Lock. O registry nome->resource (named_ptr) tem seu
  // próprio mutex, independente do mutex de cada resource (fila/closed).
  class Mailbox {

    class resource {
      friend Mailbox;
      std::mutex m_mutex;
      std::condition_variable m_condition;
      std::queue<std::string> m_queue;
      bool m_closed = false;
    };

    private:
    named_ptr<Mailbox::resource> m_resource;

    public:
    Mailbox(const char * name);
    Mailbox(const Mailbox &obj);
    ~Mailbox();

    // não bloqueante; lança (throw const char *) se a Mailbox estiver
    // fechada.
    void send(const std::string & message);

    // bloqueia até existir mensagem ou a Mailbox ser fechada; nullopt
    // significa fechada e sem mensagens.
    std::optional<std::string> receive();

    // nunca bloqueia; nullopt significa fila vazia no momento da chamada.
    std::optional<std::string> fetch();

    bool isClosed();

    // idempotente; acorda consumidores bloqueados em receive().
    void close();

  };

} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_MAILBOX_
