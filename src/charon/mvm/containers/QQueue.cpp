// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/mvm>
#include <QQueue>

using namespace charon;

static QQueue<mvm::data *> * s_container = new QQueue<mvm::data *>;

void mvm::container::init()
{

}

void mvm::container::push(mvm::data * data)
{
  s_container->enqueue(data);
}

void mvm::container::back(mvm::data * data)
{
  s_container->enqueue(data);
}

mvm::data * mvm::container::pop()
{
  return s_container->dequeue();
}

bool mvm::container::empty()
{
  return s_container->empty();
}
