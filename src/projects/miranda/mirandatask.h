// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef MIRANDATASK_H
#define MIRANDATASK_H

#include <QObject>
#include <QRunnable>

class MirandaTask : public QObject, public QRunnable
{

public:
  MirandaTask(qintptr descriptor);

protected:
  qintptr m_descriptor;
  void run();
};

#endif // MIRANDATASK_H
