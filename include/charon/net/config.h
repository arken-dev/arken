// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>

#ifndef _ARKEN_NET_CONFIG_
#define _ARKEN_NET_CONFIG_

namespace arken {
namespace net {

class Config
{
  private:
  std::string  m_address;
  std::string  m_pid;
  std::string  m_log;
  unsigned int m_port;
  unsigned int m_threads;
  bool         m_service;

  public:
  Config(string path);
  std::string address();
  std::string pid();
  unsigned int port();
  unsigned int threads();
  bool service();
};

}
}

#endif
