#ifndef MIRANDATASK_H
#define MIRANDATASK_H

#include <luajit-2.0/lua.hpp>

#include <QRunnable>
#include <QTcpSocket>
#include <QObject>
#include <QStack>
#include <QByteArray>
#include "mirandaserver.h"

class MirandaServer;

class MirandaTask : public QObject, public QRunnable
{
  Q_OBJECT

public:
  MirandaTask(MirandaServer * server, qintptr descriptor);
  QByteArray httpStatus(int code);

protected:
  QByteArray   m_oberonPath;
  QByteArray   m_profilePath;
  qintptr      m_descriptor;
  void run();
  void parseRequest(lua_State * mState, QByteArray &buffer);
  QByteArray processRequest(lua_State * mState);
};

#endif // MIRANDATASK_H
