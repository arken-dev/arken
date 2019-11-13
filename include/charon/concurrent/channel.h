// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_CONCURRENT_CHANNEL_
#define _CHARON_CONCURRENT_CHANNEL_

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <string>
#include <condition_variable>

namespace charon {
namespace concurrent {

  class channel {

    private:
    static std::vector<std::thread> * workers;
    static std::queue<channel *> * queue;
    static std::mutex * mtx;
    static std::condition_variable * condition;
    static uint32_t max;
    static uint32_t actives;

    std::queue<std::string> * m_read;
    std::queue<std::string> * m_write;

    std::mutex * m_read_mtx;
    std::mutex * m_write_mtx;

    std::condition_variable * m_read_condition;
    std::condition_variable * m_write_condition;

    string m_params;
    string m_fileName;
    bool m_finished;
    channel * m_client;
    std::function<void( channel * )> m_function;
    void run();

    public:
    channel(
      std::queue<std::string> * read,
      std::queue<std::string> * write,
      std::mutex * read_mtx,
      std::mutex * write_mtx,
      std::condition_variable * read_condition,
      std::condition_variable * write_condition
    );

    channel( const char * fileName, const char * params );
    channel( void (*func)( channel * channel ), const char * params );

    ~channel();
    bool empty();
    void write(std::string message);
    std::string read();

    static void working();
    static channel * get(); // private ???
    static channel * start(const char * fileName, const char * params);
    static channel * start(void (* func)( channel *), const char * params);
    static channel * push(channel * c); // private ???
    static void wait();
    static void set(uint32_t max);

  };

} // namespace concurrent
} // namespace charon
#endif // _CHARON_CONCURRENT_CHANNEL_
