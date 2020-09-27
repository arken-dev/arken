// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_TASK_
#define _ARKEN_TASK_

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <string>
#include <condition_variable>

namespace arken
{

  class task {
    private:

    class work {
      std::string m_uuid;
      std::string m_fileName;
      public:
      const char * uuid();
      const char * fileName();
      work(const char * uuid, const char * fileName);
    };

    static std::vector<std::thread> * workers;
    static std::queue<task::work *> * queue;
    static std::mutex * mtx;
    static std::condition_variable * condition;
    static uint32_t max;
    static uint32_t actives;

    task() {};
    ~task() {};
    static void run();

    public:
    static task::work * get();
    static char * start( const char * fileName, const char * data );
    static const char * value( const char * uuid );
    static void insert( const char * uuid, const char * data );
    static void wait();
    static void set(uint32_t max);

  };

}

#endif // _ARKEN_TASK_
