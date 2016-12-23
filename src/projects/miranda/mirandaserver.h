// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef MIRANDASERVER_H
#define MIRANDASERVER_H

#include <QObject>
#include <QDebug>
#include <QThreadPool>
#include <QTcpServer>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include "mirandatask.h"

class MirandaTask;

class MirandaServer : public QTcpServer
{

public:
  explicit MirandaServer(QCoreApplication *app);
  void incomingConnection(qintptr descriptor);
  void start();

private:
  QThreadPool * m_pool;
  quint16       m_port;
  QString       m_address;
  QString       m_pid;
  int           m_maxThreadCount;

};

#endif // MIRANDASERVER_H
