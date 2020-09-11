// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_CONCURRENT_TRITON_
#define _CHARON_CONCURRENT_TRITON_

#include <charon/mvm.h>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <string>

namespace charon {
namespace concurrent {

  class triton : public Base {

    private:

    string m_params;
    string m_fileName;

    bool m_finished;
    bool m_release;
    bool m_purge;

    std::unordered_map<std::string, std::string *> m_result;
    std::unordered_map<std::string, int> m_total;
    std::queue<std::string> m_queue;
    std::mutex m_mutex;

    void run();
    bool release();
    bool purge();

    public:

    triton( const char * fileName, const char * params, bool purge );
    ~triton();

    static void start(const char * fileName, const char * params, bool purge = false);
    static void wait();
    void perform();
    void enqueue(string node);
    void append(string key, string result);
    void count(string key);
    int  total(string);
    string result(string key);

    class node : public Base {

      string   m_fileName;
      triton * m_triton;
      unsigned int m_number = 0;

      void run();
      bool release();
      unsigned int number();
      const char * dequeue();

      public:
      node(triton * ptr, string fileName, int number);
    };
  };


} // namespace concurrent
} // namespace charon
#endif // _CHARON_CONCURRENT_TRITON_
