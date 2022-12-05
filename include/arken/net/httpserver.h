// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NET_HTTP_SERVER_
#define _ARKEN_NET_HTTP_SERVER_

#include <vector>
#include <arken/string.h>

namespace arken {
namespace net {

class HttpServer
{
  using string = arken::string;

  static
  string dispatcher;
  string m_address;
  string m_pid;
  std::vector<string> m_service;
  int    m_port;
  int    m_threads;

  public:

  static
  std::string handler(const char * data, size_t size);
  static const char * status(int code);

  HttpServer();
  HttpServer(const char * address, int port);
  void setThreads(int threads);
  void setPid(const char * pid);
  void setDispatcher(string dispatcher);
  void addService(string service);
  void start();

  private:
  void run();

};

} // namespace net
} // namespace arken

#endif
