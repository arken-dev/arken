// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include "mirandacache.h"

MirandaCache::MirandaCache(const char * data, int expires = -1)
{
  m_data = data;
  if( expires < 0 ) {
    m_expires = -1;
  } else {
    m_expires = os::microtime() + expires;
  }
}

MirandaCache::~MirandaCache()
{
  delete[] m_data;
}

const char * MirandaCache::data()
{
  return m_data;
}

bool MirandaCache::isExpires()
{
  if ( m_expires > 0 ) {
    return os::microtime() >= m_expires;
  } else {
    return false;
  }
}
