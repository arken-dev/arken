#include "mirandaserver.h"
#include "mirandatask.h"
#include "mirandastate.h"

MirandaServer::MirandaServer(QCoreApplication *app)
{

  QFileInfo dispatch = QFileInfo("dispatch.lua");

  if( dispatch.exists() ) {
    qDebug() << "dispatch in local dir ...";
  } else {
    if( app->arguments().size() == 1 ) {
      qDebug() << "where the dispatch.lua file ???";
      throw;
    }
    QString file_name = app->arguments().at(1);
    dispatch = QFileInfo(file_name);
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

  if(this->listen(QHostAddress::Any, 2345)) {
    qDebug() << "start miranda ...";
  } else {
    qDebug() << "fail start miranda ...";
  }
}

void MirandaServer::incomingConnection(qintptr descriptor)
{
  m_pool->start(new MirandaTask(descriptor));
}
