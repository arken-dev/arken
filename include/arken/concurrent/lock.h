// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_LOCK_
#define _ARKEN_CONCURRENT_LOCK_

#include <mutex>
#include <lua/lua.hpp>
#include <arken/named_ptr.h>
#include <arken/mvm.h>

namespace arken {
namespace concurrent {

  class Lock
  {

    using Shared = arken::mvm::Shared;

    class resource {
      friend Lock;
      Shared m_shared;
      std::mutex m_mutex;
    };

    private:
    bool m_reentrant;
    int m_count;
    named_ptr<Lock::resource> m_resource;

    public:
    Lock(const char * name, bool reentrant = false);
    Lock(const Lock &obj);
    ~Lock();
    void enable();
    void disable();
    void call(lua_State *L);
    int  pcall(lua_State *L);
    Shared shared();

  };

} // namespace concurrent
} // namespace arken

#endif
