#ifndef MIRANDASTATE_H
#define MIRANDASTATE_H

#include <luajit-2.0/lua.hpp>

#include <QCoreApplication>
#include <QByteArray>
#include <QDateTime>
#include <QMutex>
#include <QStack>
#include <QHash>

class MirandaState
{

public:

  MirandaState();
  ~MirandaState();

  static void init(QCoreApplication *app);
  static MirandaState * pop();
  static void push(MirandaState *);
  static void reload();
  static void clear();
  static int  version();
  static void insert(const char * key, const char * value);
  static const char * value(const char * key);

  lua_State * instance();

private:
  lua_State * m_State;
  qint64      m_version;
  static qint64     s_version;
  static QByteArray s_oberonPath;
  static QByteArray s_profilePath;
  static QMutex     s_mutex;
  static QStack<MirandaState *> * s_stack;
  static QHash<QByteArray, QByteArray> * s_cache;

};

#endif // MIRANDASTATE_H
