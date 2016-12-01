#include "mirandaserver.h"
#include "mirandatask.h"
#include "mirandastate.h"
#include <QFile>
#include <QJsonObject>
#include <iostream>

MirandaServer::MirandaServer(QCoreApplication *app)
{
  QFile config("config/miranda.json");
  if( config.exists() ) {
    config.open(QIODevice::ReadOnly);
    QJsonParseError * error = new QJsonParseError();
    QJsonDocument json = QJsonDocument::fromJson(config.readAll(), error);
    if( error->error != 0 ) {
      qDebug() << error->errorString();
      throw;
    }
    QJsonObject object = json.object();
    m_port =    object.value("port").toInt();
    m_address = object.value("address").toString();
    m_maxThreadCount = object.value("threads").toInt();
  } else {
    qDebug() << "config/miranda.json file not exists";
    m_port = 2345;
    m_address = "localhost";
    m_maxThreadCount = 15;
  }

  QFileInfo dispatch = QFileInfo("dispatch.lua");

  if( ! dispatch.exists() ) {
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
  m_pool->setMaxThreadCount(m_maxThreadCount);

  if(this->listen(QHostAddress(m_address), m_port)) {
    qDebug() << QString("start miranda %1:%2 (%3) threads...").arg(m_address).arg(m_port).arg(m_maxThreadCount).toLocal8Bit().data();
  } else {
    qDebug() << "fail start miranda ...";
  }
  QFile log("logs/miranda.pid");
  if ( log.open(QIODevice::WriteOnly) ) {
    log.write(QByteArray::number(os::pid()));
    log.close();
  } else {
    qDebug() << "logs/miranda.pid not open";
    throw;
  }
}

void MirandaServer::incomingConnection(qintptr descriptor)
{
  m_pool->start(new MirandaTask(descriptor));
}
