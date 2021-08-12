// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/net/httpserver.h>
#include <arken/net/config.h>
#include <arken/mvm.h>
#include <string>
#include "EventLoop.h"
#include "Server.h"
#include <getopt.h>

namespace arken {
namespace net {

void HttpServer::run()
{
  std::cout << "start arken.net.HttpServer (linux-model-1) " << m_address <<
     ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, m_threads , m_port);
    myHTTPServer.start();
    mainLoop.loop();
}

} // namespace net
} // namespace arken
