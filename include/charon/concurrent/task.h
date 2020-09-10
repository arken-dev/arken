// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_CONCURRENT_TASK_
#define _CHARON_CONCURRENT_TASK_

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <condition_variable>
#include <cstring>
#include <iostream>

namespace charon {
namespace concurrent {

  class task {

    private:
    static std::vector<std::thread> * workers;
    static std::queue<task *> * queue;
    static std::mutex * mtx;
    static std::condition_variable * condition;
    static uint32_t max;
    static uint32_t actives;

    string m_uuid;
    string m_params;
    string m_fileName;
    bool m_release;
    std::function<void( const char * )> m_function;

    void run();

    public:
    task(const char * fileName, const char * params, bool release);
    task(void (*func)( const char * uuid ), const char * params, bool release);
    ~task();
    static void working();
    static task * get();
    static string start(const char * fileName, const char * params, bool release = false);
    static string start(void (* func)( const char *), const char * params, bool release = false);
    static string push(task * task);
    static void wait();
    static void set(uint32_t max);
  };

} // namespace concurrent
} // namespace charon

#endif // _CHARON_CONCURRENT_TASK_
