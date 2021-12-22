// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/concurrent/lock.h>

namespace arken {

Lock::Lock(const char * name)
{
  m_resource = named_ptr<std::mutex>(name);
}

Lock::~Lock() = default;

void Lock::enable()
{
  m_resource->lock();
}

void Lock::disable()
{
  m_resource->unlock();
}

} // namespace arken
