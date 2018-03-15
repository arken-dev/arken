// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/mvm>
#include <queue>

using namespace charon;

static std::queue<mvm::data *> * s_container = new std::queue<mvm::data *>;

void mvm::container::push(mvm::data * data)
{
  s_container->push(data);
}

mvm::data * mvm::container::pop()
{
  mvm::data * data = s_container->front();
  s_container->pop();
  return data;
}

bool mvm::container::empty()
{
  return s_container->empty();
}
