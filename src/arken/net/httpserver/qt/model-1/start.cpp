// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <server.h>
#include <arken/net/httpserver.h>
#include <arken/mvm.h>
#include <QtCore>
#include <QCoreApplication>

namespace arken {
namespace net {

void HttpServer::start()
{
  std::cout << "start arken.net.HttpServer (qt-model-1) " << m_address <<
     ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

   QCoreApplication *app = new QCoreApplication(mvm::s_argc, mvm::s_argv);
   MirandaServer server(app);
   server.setPort(m_port);
   server.setThreads(m_threads);
   server.setAddress(m_address);
   if( ! m_pid.empty() ) {
     server.setPid(m_pid);
   }
   server.start();
   app->exec();
}

} // namespace net
} // namespace arken
