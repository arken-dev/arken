#include "mirandaserver.h"
#include "mirandatask.h"
#include "mirandastate.h"

MirandaServer::MirandaServer(QCoreApplication *app)
{

  QFileInfo dispatcher = QFileInfo("dispatcher.lua");

  if( dispatcher.exists() ) {
    qDebug() << "dispatcher in local dir ...";
  } else {
    dispatcher = QFileInfo(app->arguments().at(1));
    if( dispatcher.exists() ) {
      qDebug() << "work dir is: " << dispatcher.absoluteDir().absolutePath();
      QDir::setCurrent( dispatcher.absoluteDir().absolutePath() );
    } else {
      qDebug() << "file dispatcher.lua not found exit... ";
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
