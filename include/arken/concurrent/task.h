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
#include <arken/mvm.h>
#include <arken/concurrent/base.h>

namespace arken {
namespace concurrent {

  class task: public base {

    public:
    void run();

    task(const char * fileName, const char * params, bool release);
    task(const task &obj);
    static task start(const char * fileName, const char * params, bool release = false);
    ~task();

  };

} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_TASK_
