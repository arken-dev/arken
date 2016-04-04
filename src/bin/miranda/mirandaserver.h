#ifndef MIRANDASERVER_H
#define MIRANDASERVER_H

#include <QObject>
#include <QDebug>
#include <QThreadPool>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QStack>
#include <MirandaTask>
#include <QCoreApplication>

class MirandaServer : public QTcpServer
{
  Q_OBJECT
public:
  explicit MirandaServer(QCoreApplication *app);
  void incomingConnection(qintptr descriptor);

private:
  QThreadPool  * pool;
  QStack<MirandaTask *> * stack;
  QByteArray oberonPath;

};

#endif // MIRANDASERVER_H
