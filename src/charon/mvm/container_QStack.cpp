// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/mvm>
#include <QStack>

using namespace charon;

static QStack<mvm::data *> * s_container = new QStack<mvm::data *>;

void mvm::container::push(mvm::data * data)
{
  s_container->push(data);
}

mvm::data * mvm::container::pop()
{
  return s_container->pop();
}

bool mvm::container::empty()
{
  return s_container->empty();
}
