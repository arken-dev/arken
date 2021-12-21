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
#include <unordered_map>
#include <arken/mvm>
#include <arken/string.h>
#include <arken/concurrent/shared.h>

namespace arken {
namespace concurrent {
namespace task {

  class priority : public base {

    using string = arken::string;

    priority();
    ~priority();

    public:
    void run();
    static void wait();

    class node : public base {

      friend class priority;
      int m_priority{0};

      node(const char * fileName, const char * params, int priority = 0, bool purge = false);

      public:
      node();
      node(const node &obj);
      bool operator()(const priority::node &n1, const priority::node &n2);
      int  priority();
      void run();
    };

    static priority::node start(const char * fileName, const char * params, int priority = 0, bool purge = false);
    static std::priority_queue<priority::node, std::vector<priority::node>, priority::node> & priority_queue();
    static std::mutex s_mutex;
    static void push(const priority::node & node);
    static std::unordered_map<string, string> &running();

    public:
    static priority::node dequeue();
    static std::atomic<uint32_t> s_actives;
    static std::atomic<uint32_t> s_max;
    static string inspect();

  };

} // namespace task
} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_TASK_PRIORITY_
