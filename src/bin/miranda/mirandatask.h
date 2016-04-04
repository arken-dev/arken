#ifndef MIRANDATASK_H
#define MIRANDATASK_H

#include <luajit-2.0/lua.hpp>

#include <QRunnable>
#include <QTcpSocket>
#include <QHash>
#include <QStack>
#include <QObject>
#include <QByteArray>

class MirandaTask : public QObject, public QRunnable
{
  Q_OBJECT

public:
  MirandaTask(QByteArray oberonPath, QStack<MirandaTask *> * stack);
  void setDescriptor(qintptr descriptor);
  QByteArray httpStatus(int code);

public slots:
  void disconnected();

protected:
  QStack<MirandaTask *> * stack;
  QByteArray   oberonPath;
  lua_State  * m_State;
  qintptr      m_descriptor;
  void run();

};

#endif // MIRANDATASK_H
