// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef CHARONTASK_H
#define CHARONTASK_H

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <string>

namespace charon
{

  class task {
    private:

    class worker {
      char * m_uuid;
      char * m_fileName;
      public:
      char * uuid();
      char * fileName();
      worker(char * uuid,  char * fileName);
      ~worker();
    };

    static std::vector<std::thread> * workers;
    static std::queue<task::worker *> * queue;
    static std::mutex * mtx;
    task() {};
    ~task() {};
    static void run();
    public:
    static const char * start( const char * fileName, const char * data );
    static const char * value( const char * uuid );
    static void insert( const char * uuid, const char * data );

  };

}

#endif // CHARONTASK_H
