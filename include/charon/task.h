// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef CHARONTASK_H
#define CHARONTASK_H

#include <QRunnable>
#include <QThread>
#include <QMutex>

using charon::ByteArray;

namespace charon
{

  class task {
    private:
    static QThreadPool * s_pool;
    task() {};
    ~task() {};
    public:
    static char * start( const char * fileName, const char * data );
    static char * pool( const char * fileName, const char * data );
    static const char * value( const char * uuid );
    static void   insert( const char * uuid, const char * data );

    class worker: public QThread, public QRunnable
    {
      friend class task;
      ByteArray m_fileName;
      ByteArray m_data;
      ByteArray m_uuid;
      worker(const char * uuid, const char * fileName, const char * data);
      ~worker();
      void run();
    };

  };

}

#endif // CHARONTASK_H
