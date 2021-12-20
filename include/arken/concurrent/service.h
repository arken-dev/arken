// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_SERVICE_
#define _ARKEN_CONCURRENT_SERVICE_

#include <mutex>
#include <vector>
#include <unordered_map>
#include <arken/base>
#include <arken/mvm>

namespace arken {
namespace concurrent {

  class service : public base {

    static
    std::atomic<uint32_t> s_version;
    static
    std::unordered_map<string, bool> s_references;
    static
    std::vector<string> s_dirName;
    static
    std::mutex s_mutex;

    uint32_t m_version;

    service( const char * fileName, const char * params, bool purge = false);
    service(const service &obj);
    ~service();
    void run();

    public:
    static void start(const char * fileName, const char * params, bool purge = false);
    static void load(const char * dirName);
    static void run(const char * dirName);
    static void reload();
    static bool checkReload();
    bool loop(int secs);
  };

} // namespace concurrent
} // namespace arken
#endif // _ARKEN_CONCURRENT_SERVICE_
