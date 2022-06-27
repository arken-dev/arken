// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_CORE_
#define _ARKEN_CONCURRENT_CORE_

#include <queue>
#include <vector>
#include <unordered_map>

#include <mutex>
#include <thread>
#include <memory>
#include <condition_variable>

#include <arken/base>
#include <arken/concurrent/base.h>

namespace arken {
namespace concurrent {

class core
{
  using string = arken::string;

  std::vector<std::thread>       m_workers;
  std::queue<concurrent::base *> m_queue;
  std::mutex                     m_mutex;
  std::condition_variable        m_condition;
  std::atomic<uint32_t>          m_max;
  std::atomic<uint32_t>          m_actives;
  std::unordered_map<string, string> m_waiting;
  std::unordered_map<std::thread::id, concurrent::base *> m_running;

  core(uint32_t max);
  ~core();
  static core & instance();
  static void working();
  public:
  static void start(concurrent::base * pointer);
  static concurrent::base * get();
  static std::queue<concurrent::base *> & queue();
  static std::mutex               & mutex();
  static std::vector<std::thread> & workers();
  static std::condition_variable  & condition();
  static std::atomic<uint32_t>    & max();
  static std::atomic<uint32_t>    & actives();
  static std::unordered_map<string, string> & waiting();
  static std::unordered_map<std::thread::id, concurrent::base *> & running();

};

} // namespace arken
} // namespace concurrent


#endif // _ARKEN_CONCURRENT_CORE_
