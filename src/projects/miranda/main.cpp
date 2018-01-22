// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <QDebug>
#include <QtCore>
#include <QCoreApplication>
#include "mirandaserver.h"
#include <charon/base>

using charon::mvm;

int main(int argc, char * argv[])
{

   mvm::init(argc, argv);
   QCoreApplication *app = new QCoreApplication(argc, argv);
   MirandaServer server(app);
   server.start();

   return app->exec();
}
