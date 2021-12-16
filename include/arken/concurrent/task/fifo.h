// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_TASK_FIFO_
#define _ARKEN_CONCURRENT_TASK_FIFO_

#include <queue>
#include <mutex>
#include <atomic>
#include <vector>
#include <memory>
#include <arken/mvm>
#include <arken/string.h>
#include <arken/concurrent/shared.h>

namespace arken {
namespace concurrent {
namespace task {

  class fifo : public Base {

    using mvm    = arken::mvm;
    using string = arken::string;
    using Shared = arken::concurrent::Shared;

    fifo();
    ~fifo();

    public:
    void run();
    static void wait();

    class node {

      friend class fifo;


      string m_uuid;
      string m_fileName;
      string m_params;
      double m_microtime{0};
      bool   m_purge{false};
      Shared m_shared;
      std::shared_ptr<std::atomic<bool>>  m_finished;

      node(const char * fileName, const char * params, bool purge = false);

      public:
      node();
      node(const node &obj);
      operator bool() const;
      void run();
      string uuid();
      double microtime();
      Shared shared();
      bool finished();
      void wait();
    };

    static fifo::node start(const char * fileName, const char * params, bool purge = false);
    static std::queue<fifo::node> & fifo_queue();
    static std::mutex s_mutex;
    static void push(const fifo::node & node);

    public:
    static fifo::node dequeue();
    static std::atomic<uint32_t> s_actives;
    static std::atomic<uint32_t> s_max;

  };

} // namespace task
} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_TASK_FIFO_
