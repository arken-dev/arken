#include "mirandaserver.h"
#include "mirandatask.h"


MirandaServer::MirandaServer(QCoreApplication *app)
{
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
  m_pool->start(new MirandaTask(this, descriptor));
}

QByteArray MirandaServer::oberonPath()
{
  return m_oberonPath;
}

QByteArray MirandaServer::profilePath()
{
  return m_profilePath;
}
