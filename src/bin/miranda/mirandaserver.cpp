#include "mirandaserver.h"
#include "mirandatask.h"
#include "mirandastate.h"

MirandaServer::MirandaServer(QCoreApplication *app)
{
  MirandaState::init(app);
}

void MirandaServer::start()
{
  m_pool = new QThreadPool(this);
  m_pool->setMaxThreadCount(15);

  if(this->listen(QHostAddress::Any, 3000)) {
    qDebug() << "start miranda ...";
  } else {
    qDebug() << "fail start miranda ...";
  }
}

void MirandaServer::incomingConnection(qintptr descriptor)
{
  m_pool->start(new MirandaTask(descriptor));
}
