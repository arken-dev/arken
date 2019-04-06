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
  std::string  m_address;
  std::string  m_pid;
  std::string  m_port;
  unsigned int m_threads;
  bool         m_service;

  public:
  Config(string path);
  std::string address();
  std::string pid();
  std::string port();
  int  threads();
  bool service();
};

}
}

#endif
