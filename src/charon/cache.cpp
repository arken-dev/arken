// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/cache>

using namespace charon;

QMutex cache::s_mutex;
QHash<ByteArray, cache::data *> * cache::s_cache = new QHash<ByteArray, cache::data *>;

const char * cache::value(const char * key)
{
  QMutexLocker ml(&s_mutex);
  cache::data * cache = s_cache->value(key, 0);

  if( cache == 0 ) {
    return 0;
  }

  if ( cache->isExpires() ) {
    s_cache->remove(key);
    return 0;
  } else {
    return cache->value();
  }
}

void cache::insert(const char *key, const char * value, int expires)
{
  QMutexLocker ml(&s_mutex);
  s_cache->insert(key, new data(value, expires));
}

int cache::remove(const char * key)
{
  QMutexLocker ml(&s_mutex);
  return s_cache->remove(key);
}

cache::data::data(const char * value, int expires)
{
  m_value = value;
  if( expires < 0 ) {
    m_expires = -1;
  } else {
    m_expires = os::microtime() + expires;
  }
}

cache::data::~data()
{
  delete[] m_value;
}

const char * cache::data::value()
{
  return m_value;
}

bool cache::data::isExpires()
{
  if ( m_expires > 0 ) {
    return os::microtime() >= m_expires;
  } else {
    return false;
  }
}
