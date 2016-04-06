#ifndef MIRANDATASK_H
#define MIRANDATASK_H

#include <luajit-2.0/lua.hpp>

#include <QRunnable>
#include <QTcpSocket>
#include <QHash>
#include <QStack>
#include <QObject>
#include <QByteArray>
#include "mirandaserver.h"

class MirandaServer;

class MirandaTask : public QObject, public QRunnable
{
  Q_OBJECT

public:
  MirandaTask(MirandaServer * server, qintptr descriptor);
  QByteArray httpStatus(int code);

public slots:
  void disconnected();

protected:
  QByteArray   m_oberonPath;
  QByteArray   m_profilePath;
  qintptr      m_descriptor;
  void run();
  void parseRequest(lua_State * mState, QByteArray &buffer);

};

#endif // MIRANDATASK_H
