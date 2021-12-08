// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_TASK_PRIORITY_
#define _ARKEN_CONCURRENT_TASK_PRIORITY_

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

  class priority : public Base {

    using mvm    = arken::mvm;
    using string = arken::string;
    using Shared = arken::concurrent::Shared;

    priority();
    ~priority();

    public:
    void run();
    static void wait();

    class node {

      friend class priority;

      string m_uuid;
      string m_fileName;
      string m_params;
      int    m_priority{0};
      double m_microtime{0};
      bool   m_purge{false};
      Shared m_shared;
      std::shared_ptr<std::atomic<bool>>  m_ref_bool;

      node(const char * fileName, const char * params, int priority = 0, bool purge = false);

      public:
      node();
      node(const node &obj);
      bool operator()(const priority::node &n1, const priority::node &n2);
      operator bool() const;
      void run();
      string uuid();
      int    priority();
      double microtime();
      Shared shared();
      bool   finished();
      void   wait();
    };

    static priority::node start(const char * fileName, const char * params, int priority = 0, bool purge = false);
    static std::priority_queue<priority::node, std::vector<priority::node>, priority::node> & priority_queue();
    static std::mutex s_mutex;
    static void push(const priority::node & node);


    public:
    static priority::node dequeue();
    static std::atomic<uint32_t> s_actives;
    static std::atomic<uint32_t> s_max;

  };

} // namespace task
} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_TASK_PRIORITY_
