// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_TASK_SINGULAR_
#define _ARKEN_CONCURRENT_TASK_SINGULAR_

#include <queue>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <memory>
#include <map>
#include <arken/mvm.h>
#include <arken/string.h>
#include <arken/concurrent/base.h>

namespace arken {
namespace concurrent {
namespace task {

  class singular : public base {

    using string = arken::string;

    singular();

    public:
    void run();

    class node : public base {
      using string = arken::string;

      friend class singular;
      string m_name;

      node(const char * fileName, const char * params, const char * name, bool release = false);

      public:
      node();
      node(const node &obj);
      void run();
      string name();
    };

    static singular::node start(const char * fileName, const char * params,
      const char * name = "default", bool release = false);
    static singular::node emplace(const char * fileName, const char * params,
      const char * name = "default", bool release = false);
    static singular::node place(const char * fileName, const char * params,
      const char * name = "default", bool release = false);
    static std::vector<string> &vector();
    static std::unordered_map<string, std::queue<singular::node>> &map();
    static std::unordered_map<string, std::atomic<bool>> &runners();
    static std::unordered_map<string, singular::node> &running();
    static std::atomic<size_t> &position();
    static std::mutex & mutex();
    static void push(const singular::node & node);

    public:
    static singular::node dequeue();
    static std::atomic<uint32_t> & actives();
    static std::atomic<uint32_t> & max();
    static string inspect();

  };

} // namespace task
} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_TASK_SINGULAR_
