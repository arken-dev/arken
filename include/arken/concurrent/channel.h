// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_CHANNEL_
#define _ARKEN_CONCURRENT_CHANNEL_

#include <arken/base>
#include <arken/mvm>
#include <arken/concurrent/shared.h>
#include <memory>
#include <queue>

namespace arken {
namespace concurrent {

  class channel : public Base {

    using Shared = arken::concurrent::Shared;

    private:

    std::shared_ptr<std::queue<std::string>> m_read;
    std::shared_ptr<std::queue<std::string>> m_write;

    std::shared_ptr<std::mutex> m_read_mtx;
    std::shared_ptr<std::mutex> m_write_mtx;

    std::shared_ptr<std::condition_variable> m_read_condition;
    std::shared_ptr<std::condition_variable> m_write_condition;


    channel * m_client;


    void run();

    public:

    channel(
      std::shared_ptr<std::queue<std::string>> read,
      std::shared_ptr<std::queue<std::string>> write,
      std::shared_ptr<std::mutex> read_mtx,
      std::shared_ptr<std::mutex> write_mtx,
      std::shared_ptr<std::condition_variable> read_condition,
      std::shared_ptr<std::condition_variable> write_condition,
      string uuid,
      std::shared_ptr<std::atomic<bool>> finished,
      Shared shared
    );

    channel(const char * fileName, const char * params, bool purge = false);
    channel(const channel &obj);
    ~channel();

    bool empty();
    void write(std::string message);
    std::string read();
    channel * client();

    static channel * start(const char * fileName, const char * params, bool purge = false);

  };

} // namespace concurrent
} // namespace arken
#endif // _ARKEN_CONCURRENT_CHANNEL_
