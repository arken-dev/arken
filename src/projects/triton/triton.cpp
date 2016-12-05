// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <triton.h>
#include <QDebug>
#include <charon/helper>
#include <CStringList>

QHash<QByteArray, QByteArray *> * Triton::s_result = new QHash<QByteArray, QByteArray *>();
QHash<QByteArray, int> * Triton::s_total = new QHash<QByteArray, int>();
QQueue<QByteArray *> * Triton::s_queue = new QQueue<QByteArray *>();
QMutex Triton::s_mutex;

Triton::Triton(int argc, char * argv[], const char * path, QByteArray fileName)
{
  m_argc = argc;
  m_argv = argv;
  m_path = path;
  m_fileName = fileName;

  m_state = Charon::init(m_argc, m_argv, m_path);
  triton_register(m_state);
}

Triton::~Triton()
{

}

void Triton::run()
{

  lua_settop(m_state, 0);
  lua_getglobal(m_state, "CHARON_PATH");

  int rv;
  rv = luaL_loadfile(m_state, m_fileName);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_state, -1));
    throw;
  }

  rv = lua_pcall(m_state, 0, 0, lua_gettop(m_state) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_state, -1));
    throw;
  }

  while( true ) {
    QByteArray * file_name = dequeue();
    if( file_name == 0 ) {
      break;
    }
    lua_settop(m_state, 0);
    lua_getglobal(m_state, "triton_run");

    lua_pushstring(m_state, *file_name);

    if( lua_pcall(m_state, 1, 0, lua_gettop(m_state) - 1 ) != 0 ) {
      fprintf(stderr, "%s\n", lua_tostring(m_state, -1));
      throw;
    }
  }

}

void Triton::init(QStringList list)
{
  if( s_queue == 0 ) {
    s_queue = new QQueue<QByteArray *>();
    for(int i=1; i < list.size(); i++) {
      QFileInfo fileInfo(list.at(i));
      if( fileInfo.isDir() ) {
        CStringList * l = os::glob(list.at(i).toLocal8Bit().data(), ".*.lua$", true);
        for(int j=0; j < l->size(); j++) {
          s_queue->append(new QByteArray(l->at(j)));
        }
      } else {
        s_queue->append(new QByteArray(list.at(i).toLocal8Bit()));
      }
    }
  }
}

QByteArray * Triton::dequeue()
{

  QMutexLocker ml(&s_mutex);

  if( s_queue->empty() ) {
    return 0;
  } else {
    return s_queue->dequeue();
  }
}

void Triton::append(const char * key, const char * value)
{
  QMutexLocker ml(&s_mutex);
  QByteArray * v = s_result->value(key, new QByteArray);
  v->append(value);
  s_result->insert(key, v);
}

void Triton::enqueue(const char * path)
{
  s_queue->enqueue(new QByteArray(path));
}

QByteArray * Triton::result(const char * key)
{
  return s_result->value(key, new QByteArray);
}

void Triton::count(const char * label)
{
  int count;
  QMutexLocker ml(&s_mutex);

  count = s_total->value(label, 0);
  count++;
  s_total->insert(label, count);
}

int Triton::total(const char * label)
{
  QMutexLocker ml(&s_mutex);
  return s_total->value(label);
}
