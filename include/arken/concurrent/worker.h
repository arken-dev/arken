// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_WORKER_
#define _ARKEN_CONCURRENT_WORKER_

#include <arken/base>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <string>
#include <arken/concurrent/base.h>
#include <arken/concurrent/core.h>

namespace arken {
namespace concurrent {

  class worker : public base {

    using string = arken::string;

    private:

    std::shared_ptr<std::atomic<int>> m_progress;
    std::shared_ptr<std::atomic<int>> m_total;
    std::queue<string> m_queue;
    std::mutex m_mutex;

    void run();

    public:
    worker(const char * fileName, const char * params, bool purge);
    worker(const worker &obj);
    ~worker();

    static worker start(const char * fileName, const char * params, bool purge = false);
    void perform(unsigned int cores);
    void enqueue(string && node);
    static std::atomic<uint32_t> s_max;
    void   increment();
    float  progress();

    public:
    class node : public base {

      using string = arken::string;

      friend class worker;

      worker * m_worker;
      uint32_t m_number = 0;

      string dequeue();
      node(worker * ptr, string fileName, uint32_t number);

      public:
      void run() override;
      uint32_t number();
      bool release() override;
      worker master();

    };

  };


} // namespace concurrent
} // namespace arken
#endif // _ARKEN_CONCURRENT_WORKER_
