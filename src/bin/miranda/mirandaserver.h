#ifndef MIRANDASERVER_H
#define MIRANDASERVER_H

#include <QObject>
#include <QDebug>
#include <QThreadPool>
#include <QTcpServer>
#include <MirandaTask>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

class MirandaTask;

class MirandaServer : public QTcpServer
{

public:
  explicit MirandaServer(QCoreApplication *app);
  void incomingConnection(qintptr descriptor);
  void start();

private:
  QThreadPool * m_pool;

};

#endif // MIRANDASERVER_H
