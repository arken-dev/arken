#ifndef MIRANDASTATE_H
#define MIRANDASTATE_H

#include <lua/lua.hpp>

#include <QCoreApplication>
#include <QDateTime>
#include <QMutex>
#include <QStack>
#include <QHash>

#include <charon/helper>
#include <OByteArray>

#include "mirandaservice.h"
#include "mirandacache.h"

class MirandaState
{

public:

  MirandaState();
  ~MirandaState();

  static void init(QCoreApplication *app);
  static MirandaState * pop();
  static MirandaState * takeFirst();
  static void push(MirandaState *);
  static void reload();
  static void clear();
  static int  version();
  static int  gc();
  static void insert(const char * key, const char * value);
  static void insert(const char * key, const char * value, int expires);
  static const char * value(const char * key);
  static int remove(const char * key);
  static void servicesLoad();
  static void servicesReload();
  static void servicesAppend(MirandaService *service);
  static void createService(QByteArray fileName);
  static void createTask(QByteArray fileName, const char * uuid);
  static void taskPool(QByteArray fileName, const char * uuid);
  static QHash<OByteArray, MirandaCache *> * s_cache;

  lua_State * instance();

private:
  lua_State * m_State;
  qint64      m_version;
  int         m_gc;
  static int        s_gc;
  static qint64     s_version;
  static OByteArray s_charonPath;
  static OByteArray s_profilePath;
  static QMutex     s_mutex;
  static QStack<MirandaState *> * s_stack;
  static QList<MirandaService*> * s_service;
  static QThreadPool * s_pool;

};

#endif // MIRANDASTATE_H
