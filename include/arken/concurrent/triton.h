// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_TRITON_
#define _ARKEN_CONCURRENT_TRITON_

#include <arken/mvm.h>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <string>

namespace arken {
namespace concurrent {

  class triton : public Base {

    private:

    string m_uuid;
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

    static string start(const char * fileName, const char * params, bool purge = false);
    static void wait();
    void perform(unsigned int cores);
    void enqueue(string node);
    void append(string key, string result);
    void count(string key);
    int  total(string);
    string result(string key);
    string uuid();

    public:
    class node : public Base {
      friend class triton;
      string   m_fileName;
      triton * m_triton;
      unsigned int m_number = 0;

      bool release();
      const char * dequeue();
      void run();
      node(triton * ptr, string fileName, int number);

      public:
      unsigned int number();
      int  total(string);
      void count(string key);
      void append(string key, string result);
      string result(string key);
      string uuid();


    };

    static void working(node * n);
  };


} // namespace concurrent
} // namespace arken
#endif // _ARKEN_CONCURRENT_TRITON_
