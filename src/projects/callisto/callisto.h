// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _PUCK_HEADER_
#define _PUCK_HEADER_

#include <lua/lua.hpp>

#include <dialog.h>

#include <QObject>
#include <QApplication>
#include <QString>
#include <QFileSystemWatcher>

class Callisto : public QObject
{

  Q_OBJECT
private:
  lua_State * m_luaState;
  QString m_file;
  int m_ref;
  void run();

public slots:
  void showFileModified(const QString &str);
  void showDirectoryModified(const QString &str);

public:
  Callisto(int argc, char * argv[], const char * path, QObject *parent = 0);
  ~Callisto();
  QFileSystemWatcher * m_watcher;
  Dialog * m_dialog;
};

#endif
