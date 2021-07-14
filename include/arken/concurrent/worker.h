// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_WORKER_
#define _ARKEN_CONCURRENT_WORKER_

#include <arken/base>
#include <arken/mvm>
#include <arken/concurrent/shared.h>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <string>

namespace arken {
namespace concurrent {

  class worker : public Base {

    using Shared = arken::concurrent::Shared;

    private:

    string m_uuid;
    string m_params;
    string m_fileName;

    bool m_finished;
    bool m_release;
    bool m_purge;

    std::queue<string> m_queue;
    std::mutex m_mutex;
    Shared m_shared;

    bool release();
    bool purge();
    void run();

    public:
    worker( const char * fileName, const char * params, bool purge );
    ~worker();

    static string start(const char * fileName, const char * params, bool purge = false);
    static void wait();
    void perform(unsigned int cores);
    void enqueue(string && node);
    string uuid();
    Shared shared();
    static std::atomic<uint32_t> s_max;


    public:
    class node : public Base {
      friend class worker;

      string   m_fileName;
      worker * m_worker;
      uint32_t m_number = 0;
      Shared   m_shared;

      bool release();
      string dequeue();
      node(worker * ptr, string fileName, uint32_t number);

      public:
      void run();
      uint32_t number();
      string uuid();
      Shared shared();

    };

  };


} // namespace concurrent
} // namespace arken
#endif // _ARKEN_CONCURRENT_WORKER_
