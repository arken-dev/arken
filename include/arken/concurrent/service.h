// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_SERVICE_
#define _ARKEN_CONCURRENT_SERVICE_

#include <arken/string.h>
#include <arken/mvm.h>

using string = arken::string;
using mvm    = arken::mvm;

namespace arken {
namespace concurrent {

  class service : public Base {

    uint32_t m_version;

    bool m_purge;
    bool m_release;

    string m_fileName;
    string m_params;

    service( const char * fileName, const char * params, bool purge );
    ~service();
    bool release();
    void run();

    public:
    static void start(const char * fileName, const char * params, bool purge = false);
    static void load(const char * dirName);
    static void wait();
    bool loop(int secs);
  };

} // namespace concurrent
} // namespace arken
#endif // _ARKEN_CONCURRENT_SERVICE_
