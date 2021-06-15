// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <QDebug>
#include <QFile>
#include <server.h>
#include <task.h>
#include <arken/base>

MirandaServer::MirandaServer(QCoreApplication *app)
{
  m_address = "127.0.0.1";
  m_pid     = "tmp/pid/server.pid";
  m_threads = os::cores();
}

void MirandaServer::start()
{
  m_pool = new QThreadPool(this);
  m_pool->setMaxThreadCount(m_threads);

  if(! this->listen(QHostAddress(m_address), m_port)) {
    qDebug() << "fail start httpserver...";
    throw;
  }
  QFile log(m_pid);
  if ( log.open(QIODevice::WriteOnly) ) {
    log.write(QByteArray::number((qint64) os::pid()));
    log.close();
  } else {
    qDebug() << m_pid << " not open";
    throw;
  }
}

void MirandaServer::incomingConnection(qintptr descriptor)
{
  m_pool->start(new MirandaTask(descriptor));
}

void MirandaServer::setAddress(const char * address)
{
  m_address = address;
}

void MirandaServer::setPort(int port)
{
  m_port = port;
}

void MirandaServer::setThreads(int threads)
{
  m_threads = threads;
}

void MirandaServer::setPid(const char * pid)
{
  m_pid = pid;
}
