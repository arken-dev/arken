// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_NAIAD_
#define _ARKEN_CONCURRENT_NAIAD_

#include <arken/mvm>
#include <queue>
#include <vector>
#include <mutex>
#include <atomic>
#include <arken/string.h>

using string = arken::string;

namespace arken {
namespace concurrent {

  class naiad : public Base {

    naiad();
    ~naiad();

    public:
    void run();
    bool release();
    static void wait();

    class node {
      friend class naiad;

      string m_uuid;
      string m_fileName;
      string m_params;
      int    m_priority{0};
      double m_microtime{0};
      bool   m_purge{false};

      node(const char * fileName, const char * params, int priority = 0, bool purge = false);

      public:
      node();
      node(const node &obj);
      bool operator()(const naiad::node &n1, const naiad::node &n2);
      operator bool() const;
      void run();
      string uuid();
      int    priority();
      double microtime();
    };

    static naiad::node start(const char * fileName, const char * params, int priority = 0, bool purge = false);
    static std::priority_queue<naiad::node, std::vector<naiad::node>, naiad::node> & priority_queue();
    static std::mutex s_mutex;
    static void push(const naiad::node & node);


    public:
    static naiad::node dequeue();
    static std::atomic<int> s_actives;
    static std::atomic<int> s_max;

  };

} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_NAIAD_
