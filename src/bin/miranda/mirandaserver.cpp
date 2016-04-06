#include "mirandaserver.h"
#include "mirandatask.h"


MirandaServer::MirandaServer(QCoreApplication *app)
{
  stack  = new QStack<MirandaTask *>;
  m_pool = new QThreadPool(this);
  m_pool->setMaxThreadCount(15);

  m_oberonPath = app->applicationDirPath().toLocal8Bit();
  m_oberonPath.truncate( m_oberonPath.lastIndexOf('/') );

  m_profilePath = m_oberonPath;
  m_profilePath.append("/profile.lua");

  if(this->listen(QHostAddress::Any, 3000)) {
    qDebug() << "start miranda ...";
  } else {
    qDebug() << "fail start miranda ...";
  }
}

void MirandaServer::incomingConnection(qintptr descriptor)
{
  MirandaTask * task;
  if( stack->isEmpty() ) {
    qDebug() << "stack is empty";
    task = new MirandaTask(this, stack);
    task->setAutoDelete(false);
  } else {
    task = stack->pop();
  }

  task->setDescriptor(descriptor);
  m_pool->start(task);
}

QByteArray MirandaServer::oberonPath()
{
  return m_oberonPath;
}

QByteArray MirandaServer::profilePath()
{
  return m_profilePath;
}
