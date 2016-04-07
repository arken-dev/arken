#ifndef MIRANDATASK_H
#define MIRANDATASK_H

#include <luajit-2.0/lua.hpp>

#include <QRunnable>
#include <QTcpSocket>
#include <QObject>
#include <QStack>
#include <QByteArray>
#include "mirandaserver.h"
#include "mirandastate.h"

class MirandaServer;

class MirandaTask : public QObject, public QRunnable
{

public:
  MirandaTask(qintptr descriptor);
  QByteArray httpStatus(int code);

protected:
  qintptr      m_descriptor;
  void run();
  void parseRequest(MirandaState * state, QByteArray &buffer);
  void processRequest(MirandaState * state, QByteArray &buffer);
};

#endif // MIRANDATASK_H
