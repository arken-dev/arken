// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/mvm>
#include <QQueue>

using namespace arken;

static QMutex s_mutex;
static QQueue<mvm::data *> * s_container = new QQueue<mvm::data *>;

void mvm::container::init()
{

}

void mvm::container::push(mvm::data * data)
{
  QMutexLocker ml(&s_mutex);
  s_container->enqueue(data);
}

void mvm::container::back(mvm::data * data)
{
  QMutexLocker ml(&s_mutex);
  s_container->enqueue(data);
}

mvm::data * mvm::container::pop()
{
  QMutexLocker ml(&s_mutex);
  return s_container->dequeue();
}

bool mvm::container::empty()
{
  QMutexLocker ml(&s_mutex);
  return s_container->empty();
}
