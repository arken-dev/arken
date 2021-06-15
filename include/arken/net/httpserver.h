// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NET_HTTP_SERVER_
#define _ARKEN_NET_HTTP_SERVER_

#include <arken/string.h>

namespace arken {
namespace net {

class HttpServer
{
  using string = arken::string;

  string m_address;
  string m_pid;
  int    m_port;
  int    m_threads;

  public:
  HttpServer(const char * address, int port);
  void setThreads(int threads);
  void setPid(const char * pid);
  void start();

};

} // namespace net
} // namespace arken

#endif
