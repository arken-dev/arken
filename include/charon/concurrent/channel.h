// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_CONCURRENT_CHANNEL_
#define _CHARON_CONCURRENT_CHANNEL_

#include <charon/mvm.h>

namespace charon {
namespace concurrent {

  class channel : public Base {

    private:

    std::queue<std::string> * m_read;
    std::queue<std::string> * m_write;

    std::mutex * m_read_mtx;
    std::mutex * m_write_mtx;

    std::condition_variable * m_read_condition;
    std::condition_variable * m_write_condition;

    string m_params;
    string m_fileName;

    bool m_release;
    bool m_purge;

    channel * m_client;
    void run();
    bool release();
    bool purge();

    public:

    channel(
      std::queue<std::string> * read,
      std::queue<std::string> * write,
      std::mutex * read_mtx,
      std::mutex * write_mtx,
      std::condition_variable * read_condition,
      std::condition_variable * write_condition
    );

    channel( const char * fileName, const char * params, bool release );
    ~channel();

    bool empty();
    void write(std::string message);
    std::string read();

    static channel * start(const char * fileName, const char * params, bool release = false);
    static void wait();

  };

} // namespace concurrent
} // namespace charon
#endif // _CHARON_CONCURRENT_CHANNEL_
