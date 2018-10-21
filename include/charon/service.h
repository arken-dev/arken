// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef CHARONSERVICE_H
#define CHARONSERVICE_H

#include <charon/base>
#include <charon/cache>
#include <thread>
#include <iostream>

namespace charon
{
  class service {

    private:
    static void run(char * uuid, char * fileName);
    int m_version;

    public:
    static char * start(const char * fileName);
    bool loop(int secs);
    service();

  };
}

#endif // CHARONSERVICE_H
