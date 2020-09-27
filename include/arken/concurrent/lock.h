// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_LOCK_
#define _ARKEN_LOCK_

#include <mutex>
#include <unordered_map>

namespace arken
{

  class Lock
  {

    private:
    std::string m_index;

    static std::unordered_map<std::string, int>           m_references;
    static std::unordered_map<std::string, std::mutex  *> m_containers;

    public:
    Lock(const char * index);
    ~Lock();
    void enable();
    void disable();

  };

} // namespace
#endif
