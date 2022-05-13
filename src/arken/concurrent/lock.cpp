// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/concurrent/lock.h>

namespace arken {
namespace concurrent {

Lock::Lock(const char * name, bool reentrant)
{
  m_resource  = named_ptr<Lock::resource>(name);
  m_reentrant = reentrant;
  m_count     = 0;
}

Lock::Lock(const Lock &obj)
{
  m_resource  = obj.m_resource;
  m_reentrant = obj.m_reentrant;
  m_count     = 0;
}

Lock::~Lock() = default;

void Lock::enable()
{

  m_count++;

  if( ! m_reentrant && m_count > 1 ) {
    m_count--;
    disable();
    throw "Lock is enable, try enabling reentrant option";
  }

  if( m_count == 1 ) {
    m_resource->m_mutex.lock();
  }
}

void Lock::disable()
{
  m_count--;

  if ( m_count < 0 ) {
    throw "Lock is not enable";
  }

  if( m_count == 0 ) {
    m_resource->m_mutex.unlock();
  }
}

Shared Lock::shared()
{
  return m_resource->m_shared;
}

} // namespace concurrent
} // namespace arken
