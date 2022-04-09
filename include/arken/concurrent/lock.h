// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_LOCK_
#define _ARKEN_LOCK_

#include <mutex>
#include <arken/named_ptr.h>
#include <arken/concurrent/shared.h>

namespace arken {
namespace concurrent {

  class Lock
  {

    using Shared = arken::concurrent::Shared;

    class resource {
      friend Lock;
      Shared m_shared;
      std::mutex m_mutex;
    };

    private:
    named_ptr<Lock::resource> m_resource;

    public:
    Lock(const char * name);
    ~Lock();
    void enable();
    void disable();
    Shared shared();

  };

} // namespace concurrent
} // namespace arken

#endif
