#include "mirandaserver.h"
#include "mirandatask.h"


MirandaServer::MirandaServer(QCoreApplication *app)
{
  stack = new QStack<MirandaTask *>;
  pool  = new QThreadPool(this);
  pool->setMaxThreadCount(15);

  oberonPath = app->applicationDirPath().toLocal8Bit();
  oberonPath.truncate( oberonPath.lastIndexOf('/') );

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
    task = new MirandaTask(oberonPath, stack);
    task->setAutoDelete(false);
  } else {
    task = stack->pop();
  }

  task->setDescriptor(descriptor);
  pool->start(task);
}
