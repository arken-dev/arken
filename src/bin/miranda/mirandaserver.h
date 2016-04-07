#ifndef MIRANDASERVER_H
#define MIRANDASERVER_H

#include <QObject>
#include <QDebug>
#include <QThreadPool>
#include <QTcpServer>
#include <MirandaTask>
#include <QCoreApplication>

class MirandaTask;

class MirandaServer : public QTcpServer
{

public:
  explicit MirandaServer(QCoreApplication *app);
  void incomingConnection(qintptr descriptor);
  QByteArray oberonPath();
  QByteArray profilePath();

private:
  QThreadPool * m_pool;
  QByteArray    m_oberonPath;
  QByteArray    m_profilePath;

};

#endif // MIRANDASERVER_H
