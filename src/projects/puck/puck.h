#ifndef _PUCK_HEADER_
#define _PUCK_HEADER_

#include <luajit-2.0/lua.hpp>

#include <dialog.h>

#include <QObject>
#include <QApplication>
#include <QString>
#include <QFileSystemWatcher>

class Puck : public QObject
{

  Q_OBJECT
private:
  lua_State * m_luaState;
  QString m_file;
  void run();

public slots:
  void showFileModified(const QString &str);
  void showDirectoryModified(const QString &str);

public:
  Puck(QApplication &app, QObject *parent = 0);
  ~Puck();
  QFileSystemWatcher * m_watcher;
  Dialog * m_dialog;
};

#endif
