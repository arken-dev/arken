// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_SERVICE_
#define _ARKEN_SERVICE_

#include <arken/base>
#include <arken/cache>
#include <thread>
#include <iostream>
#include <mutex>
#include <vector>

namespace arken
{
  class service {

    private:
    static void run(char * uuid, char * fileName);
    uint32_t m_version;
    bool m_quit;
    static bool          s_exit;
    static uint32_t      s_version;
    static char        * s_dirName;
    static std::mutex  * s_mutex;
    static std::vector<std::string> * s_services;

    public:
    static char * start(const char * fileName);
    static void load(const char * dirName);
    static void load();
    bool loop(int secs, lua_State * state = 0);
    void quit();
    void exit();
    service();

  };
} // namespace arken

#endif // ARKENSERVICE_H
