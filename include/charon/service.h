// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef CHARONSERVICE_H
#define CHARONSERVICE_H

#include <charon/base>
#include <charon/cache>
#include <QRunnable>
#include <QThread>
#include <QMutex>

namespace charon
{
  class service {

    public:
    static char * start( const char * fileName);
    static int gc();

    class worker: public QThread, public QRunnable
    {
      friend class service;
      ByteArray m_fileName;
      ByteArray m_uuid;
      double    m_microtime;
      int       m_version;
      worker(const char * uuid, const char * fileName);
      ~worker();
      void run();
      bool isShutdown();
      double finishedAt();
      public:
      bool loop(int secs);
    };

    static QMutex s_mutex;
    static QList<worker *> * s_workers;

  };
}

#endif // CHARONSERVICE_H
