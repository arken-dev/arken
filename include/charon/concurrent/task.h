// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_CONCURRENT_TASK_
#define _CHARON_CONCURRENT_TASK_

#include <string>
#include <cstring>
#include <iostream>
#include <charon/mvm>

namespace charon {
namespace concurrent {

  class task: public Base {

    private:

    string m_uuid;
    string m_params;
    string m_fileName;
    bool m_release;
    bool m_purge;

    public:
    void run();
    bool release();
    string uuid();

    task(const char * fileName, const char * params, bool release);
    static string start(const char * fileName, const char * params, bool release = false);
    static void wait();
    ~task();

  };

} // namespace concurrent
} // namespace charon

#endif // _CHARON_CONCURRENT_TASK_
