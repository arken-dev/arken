// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/mvm>
#include <iostream>

using namespace charon;

static int s_capacity = 30;
static int s_count    = 0;
static mvm::data * s_container[30];

void mvm::container::init()
{
  std::cout << "init...";
}

void mvm::container::push(mvm::data * data)
{
  if( s_count > s_capacity ) {
    // realloc memory
  }
  //std::cout << "push " << s_count;
  s_container[s_count++] = data;
}

void mvm::container::back(mvm::data * data)
{
  //std::cout << "back ";

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
  //std::cout << "pop " << s_count;;
  mvm::data * tmp = s_container[--s_count];
  //std::cout << "pop " << s_count;;
  return tmp;
}

bool mvm::container::empty()
{
  return s_count == 0;
}
