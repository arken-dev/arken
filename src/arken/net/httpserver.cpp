// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/net/httpserver.h>
#include <arken/os.h>

namespace arken {
namespace net {

HttpServer::HttpServer(const char * address, int port)
{
  m_address = address;
  m_port    = port;
  m_threads = os::cores();
}

void HttpServer::setThreads(int threads)
{
  m_threads = threads;
}

void HttpServer::setPid(const char * pid)
{
  m_pid = pid;
}

} // namespace net
} // namespace arken
