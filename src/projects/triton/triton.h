#ifndef TRITON_H
#define TRITON_H

#include <luajit-2.0/lua.hpp>

#include <QObject>
#include <QDebug>
#include <QThreadPool>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QQueue>
#include <QMutex>
#include <QRunnable>
#include <QStringList>
#include <oberon.h>

void triton_register(lua_State * L);

class Triton : public QObject, public QRunnable
{

  private:
  int m_argc;
  char ** m_argv;
  const char * m_path;
  QByteArray m_fileName;
  lua_State * m_state;
  static QQueue<QByteArray *> * s_queue;
  static QMutex s_mutex;
  static int s_ok;
  static int s_error;
  static int s_pending;
  static QByteArray * s_result;

  public:
  Triton(int argc, char * argv[], const char * path, QByteArray fileName);
  ~Triton();
  void run();
  static void appendPath(const char * path);
  static void appendResult(const char * result);
  static void addOk();
  static void addError();
  static void addPending();
  static int  ok();
  static int  error();
  static int  pending();
  static QByteArray * result();

  QByteArray * dequeue();

  static void init(QStringList list);
};

#endif // TRITON_H
