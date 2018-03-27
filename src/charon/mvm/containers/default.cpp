// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/mvm>
#include <iostream>

using namespace charon;

static mvm::data *  s_container[30];
static mvm::data ** s_pointer;

void mvm::container::init()
{
  std::cout << "init...";
  s_pointer = s_container;
}

void mvm::container::push(mvm::data * data)
{
  //if( s_count > s_capacity ) {
    // realloc memory
  //}
  *s_pointer++ = data;
}

void mvm::container::back(mvm::data * data)
{
  /*
  int i = 0;
  mvm::data * tmp1 = data;
  mvm::data * tmp2;
  while( i < s_count ) {
    tmp2 = s_container[i];    
    s_container[i] = tmp1;
    tmp1 = tmp2;
  }
  */
}

mvm::data * mvm::container::pop()
{
  return *--s_pointer;
}

bool mvm::container::empty()
{
  return s_pointer == s_container;
}
