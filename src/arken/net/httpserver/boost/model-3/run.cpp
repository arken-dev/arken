// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "server.hpp"

#include <arken/net/httpserver.h>
#include <arken/net/config.h>
#include <arken/mvm.h>

namespace arken {
namespace net {

void HttpServer::run()
{
  try
  {
    // Initialise the server.
    std::cout << "start arken.net.HttpServer (boost-model-3) " << m_address <<
      ":" << m_port << " (" << m_threads << ") threads..." << std::endl;


    http::server3::server s(m_address.data(), std::to_string(m_port), "public", m_threads);

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }


}

} // namespace net
} // namespace arken
