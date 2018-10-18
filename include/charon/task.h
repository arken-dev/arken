// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_TASK_
#define _CHARON_TASK_

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <string>

namespace charon
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

    task() {};
    ~task() {};
    static void run();

    public:
    static char * start( const char * fileName, const char * data );
    static const char * value( const char * uuid );
    static void insert( const char * uuid, const char * data );

  };

}

#endif // _CHARON_TASK_
