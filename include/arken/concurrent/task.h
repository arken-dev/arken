// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_TASK_
#define _ARKEN_CONCURRENT_TASK_

#include <string>
#include <cstring>
#include <iostream>
#include <arken/base>
#include <arken/mvm>
#include <arken/concurrent/shared.h>

namespace arken {
namespace concurrent {

  class task: public Base {

    private:

    string m_params;
    string m_fileName;
    Shared m_shared;
    bool m_purge;

    public:
    void run();
    Shared shared();

    task(const char * fileName, const char * params, bool purge);
    task(const task &obj);
    static task start(const char * fileName, const char * params, bool purge = false);
    static void wait();
    ~task();

  };

} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_TASK_
