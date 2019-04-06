// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>

#ifndef _CHARON_NET_CONFIG_
#define _CHARON_NET_CONFIG_

namespace charon {
namespace net {

class Config
{
  private:
  string m_address;
  string m_pid;
  int    m_port;
  int    m_threads;
  bool   m_service;

  public:
  Config(string path);
  string address();
  string pid();
  int  port();
  int  threads();
  bool service();
};

}
}

#endif
