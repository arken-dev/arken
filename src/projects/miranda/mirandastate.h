// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef MIRANDASTATE_H
#define MIRANDASTATE_H

#include <lua/lua.hpp>
#include <charon/base>

#include <QCoreApplication>
#include <QDateTime>
#include <QMutex>
#include <QStack>
#include <QHash>

#include "mirandaservice.h"

using charon::ByteArray;

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
  static int  clear();
  static int  version();
  static int  gc();
  static void servicesLoad();
  static void servicesReload();
  static void servicesAppend(MirandaService *service);
  static void createService(QByteArray fileName);
  static void createTask(QByteArray fileName, const char * uuid);
  static void taskPool(QByteArray fileName, const char * uuid);

  lua_State * instance();

private:
  lua_State * m_State;
  qint64      m_version;
  int         m_gc;
  static int       s_gc;
  static qint64    s_version;
  static ByteArray s_charonPath;
  static ByteArray s_profilePath;
  static QMutex    s_mutex;
  static QStack<MirandaState *> * s_stack;
  static QList<MirandaService*> * s_service;
  static QThreadPool * s_pool;

};

#endif // MIRANDASTATE_H
