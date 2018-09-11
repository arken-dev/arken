// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef TRITON_H
#define TRITON_H

#include <lua/lua.hpp>

#include <QObject>
#include <QDebug>
#include <QThreadPool>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QQueue>
#include <QHash>
#include <QMutex>
#include <QRunnable>
#include <QStringList>
#include <charon/base>

void triton_register(lua_State * L);

class Triton : public QObject, public QRunnable
{

  private:
  int m_argc;
  char ** m_argv;
  QByteArray m_fileName;
  lua_State * m_state;
  static QHash<QByteArray, QByteArray *> * s_result;
  static QHash<QByteArray, int> * s_total;
  static QQueue<QByteArray *> * s_queue;
  static QMutex s_mutex;

  public:
  Triton(int argc, char * argv[], QByteArray fileName, int number);
  ~Triton();
  void run();
  static void init(QStringList list);
  static void enqueue(const char * path);
  static void append(const char * key, const char * result);
  static void count(const char * label);
  static int  total(const char * label);
  static QByteArray * result(char const * key);
  static QByteArray * dequeue();
};

#endif // TRITON_H
