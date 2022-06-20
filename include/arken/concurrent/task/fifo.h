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
#include <unordered_map>
#include <arken/mvm.h>
#include <arken/string.h>
#include <arken/concurrent/shared.h>

namespace arken {
namespace concurrent {
namespace task {

  class fifo : public base {

    using string = arken::string;

    fifo();
    ~fifo();

    public:
    void run();

    class node : public base {
      friend class fifo;

      node(const char * fileName, const char * params, bool purge = false);

      public:
      node();
      node(const node &obj);
      void run();
    };

    static fifo::node start(const char * fileName, const char * params, bool purge = false);
    static std::queue<fifo::node> & fifo_queue();
    static std::mutex & mutex();
    static void push(const fifo::node & node);
    static std::unordered_map<string, string> &running();

    public:
    static fifo::node dequeue();
    static std::atomic<uint32_t> & actives();
    static std::atomic<uint32_t> & max();
    static string inspect();

  };

} // namespace task
} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_TASK_FIFO_
