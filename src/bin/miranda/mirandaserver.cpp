#include "mirandaserver.h"
#include "mirandatask.h"
#include "mirandastate.h"

MirandaServer::MirandaServer(QCoreApplication *app)
{
  m_oberonPath = app->applicationDirPath().toLocal8Bit();
  m_oberonPath.truncate( m_oberonPath.lastIndexOf('/') );

  m_profilePath = m_oberonPath;
  m_profilePath.append("/profile.lua");

  MirandaState::init(m_oberonPath, m_profilePath);
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
