#include "mirandaserver.h"
#include "mirandatask.h"
#include "mirandastate.h"

MirandaServer::MirandaServer(QCoreApplication *app)
{

  QFileInfo dispatch = QFileInfo("dispatch.lua");

  if( dispatch.exists() ) {
    qDebug() << "dispatch in local dir ...";
  } else {
    dispatch = QFileInfo(app->arguments().at(1));
    if( dispatch.exists() ) {
      qDebug() << "work dir is: " << dispatch.absoluteDir().absolutePath();
      QDir::setCurrent( dispatch.absoluteDir().absolutePath() );
    } else {
      qDebug() << "file dispatch.lua not found exit... ";
      throw;
    }
  }

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
