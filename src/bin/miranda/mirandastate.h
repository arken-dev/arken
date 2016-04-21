#ifndef MIRANDASTATE_H
#define MIRANDASTATE_H

#include <luajit-2.0/lua.hpp>

#include <QCoreApplication>
#include <QDateTime>
#include <QMutex>
#include <QStack>
#include <QHash>

#include <oberon/helper>
#include <OByteArray>

#include "mirandaservice.h"

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
  static void servicesLoad();
  static void servicesReload();
  static void servicesAppend(MirandaService *service);
  static void createService(QByteArray fileName);
  static void createTask(QByteArray fileName, const char * uuid);

  lua_State * instance();

private:
  lua_State * m_State;
  qint64      m_version;
  static qint64     s_version;
  static OByteArray s_oberonPath;
  static OByteArray s_profilePath;
  static QMutex     s_mutex;
  static QStack<MirandaState *> * s_stack;
  static QList<MirandaService*> * s_service;
  static QHash<OByteArray, OByteArray> * s_cache;

};

#endif // MIRANDASTATE_H
