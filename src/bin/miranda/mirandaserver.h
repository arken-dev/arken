#ifndef MIRANDASERVER_H
#define MIRANDASERVER_H

#include <QObject>
#include <QDebug>
#include <QThreadPool>
#include <QTcpServer>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include "mirandatask.h"

class MirandaTask;

class MirandaServer : public QTcpServer
{

public:
  explicit MirandaServer(QCoreApplication *app);
  void incomingConnection(qintptr descriptor);
  void start();

private:
  QThreadPool * m_pool;
  quint16       m_port;
  QString       m_address;

};

#endif // MIRANDASERVER_H
