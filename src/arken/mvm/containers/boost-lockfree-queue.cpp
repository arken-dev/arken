// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/mvm>

// TODO spsc not works I accept your help
#include <boost/lockfree/queue.hpp>

using namespace arken;

static boost::lockfree::queue<mvm::data *> s_container{1024};

void mvm::container::init()
{

}

void mvm::container::push(mvm::data * data)
{
  s_container.push(data);
}

void mvm::container::back(mvm::data * data)
{
  s_container.push(data);
}

mvm::data * mvm::container::pop()
{
  mvm::data * data = 0;
  s_container.pop(data);
  return data;
}

bool mvm::container::empty()
{
  return s_container.empty();
}
