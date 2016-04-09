#ifndef MIRANDASTATE_H
#define MIRANDASTATE_H

#include <luajit-2.0/lua.hpp>

#include <QByteArray>
#include <QDateTime>
#include <QMutex>
#include <QStack>

class MirandaState
{

public:

  MirandaState();
  ~MirandaState();

  static void init(QByteArray oberonPath, QByteArray profilePath);
  static MirandaState * pop();
  static void push(MirandaState *);
  static void reload();
  static void clear();
  static int  version();
  lua_State * instance();

private:
  lua_State * m_State;
  qint64      m_version;
  static qint64     s_version;
  static QByteArray s_oberonPath;
  static QByteArray s_profilePath;
  static QMutex     s_mutex;
  static QStack<MirandaState *> * s_stack;
};

#endif // MIRANDASTATE_H
