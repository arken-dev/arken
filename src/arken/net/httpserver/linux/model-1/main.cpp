// Copyright 2015 Lin Ya (xxbbb@vip.qq.com)
// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <string>
#include <QJsonObject>
#include "EventLoop.h"
#include "Server.h"
#include <getopt.h>
#include <arken/mvm>
#include <arken/base>
#include <arken/log.h>
#include <arken/net/config.h>

using mvm     = arken::mvm;
using Log     = arken::Log;
using Config  = arken::net::Config;

int main(int argc, char *argv[])
{
    mvm::init(argc, argv);

    Config c("config/titan.json");
    std::cout << "start titan " << c.address() << ":" << c.port() <<
      " (" << c.threads() << ") threads..." << std::endl;

    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, c.threads() , c.port());
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}
