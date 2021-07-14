// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_TASK_SCHEDULED_
#define _ARKEN_CONCURRENT_TASK_SCHEDULED_

#include <queue>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <memory>
#include <arken/mvm>
#include <arken/string.h>
#include <arken/concurrent/shared.h>

namespace arken {
namespace concurrent {
namespace task {

  class scheduled : public Base {

    using mvm    = arken::mvm;
    using string = arken::string;
    using Shared = arken::concurrent::Shared;

    scheduled();
    ~scheduled();

    public:
    void run();
    bool release();
    static void wait();

    class node {
      friend class scheduled;

      string m_uuid;
      string m_fileName;
      string m_params;
      string m_name;
      double m_microtime{0};
      bool   m_purge{false};
      Shared m_shared;
      std::shared_ptr<std::atomic<bool>>  m_ref_bool;

      node(const char * fileName, const char * params, const char * name, bool purge = false);

      public:
      node();
      ~node();
      node(const node &obj);
      operator bool() const;
      void run();
      string uuid();
      string name();
      double microtime();
      Shared shared();
      bool finished();
    };

    static scheduled::node start(const char * fileName, const char * params,
      const char * name = "default", bool purge = false);
    static std::queue<scheduled::node> &queue();
    static std::vector<string> &vector();
    static std::unordered_map<string, std::queue<scheduled::node>> &map();
    static std::unordered_map<string, std::atomic<uint32_t>> &runners();
    static std::atomic<size_t> &position();
    static std::mutex s_mutex;
    static void push(const scheduled::node & node);

    public:
    static scheduled::node dequeue();
    static std::atomic<uint32_t> s_actives;
    static std::atomic<uint32_t> s_max;

  };

} // namespace task
} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_TASK_SCHEDULED_
