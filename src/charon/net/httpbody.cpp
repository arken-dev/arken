// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/net/httpbody.h>
#include <QFile>

using namespace charon::net;

HttpBody::HttpBody(char * buffer, size_t size)
{
  m_release = false;
  m_buffer  = buffer;
  m_size    = size;
}

HttpBody::~HttpBody()
{
  if( m_release == false ) {
    delete[] m_buffer;
  }
}

HttpBody * HttpBody::loadFile(const char *path)
{
  QFile file(path);
  QByteArray raw;

  file.open(QIODevice::ReadOnly);
  raw = file.readAll();
  char * buffer = new char[raw.size() + 1];
  memcpy( buffer, raw.data(), raw.size() );
  buffer[raw.size()] = '\0';
  return new HttpBody(buffer, raw.size());
}

HttpBody * HttpBody::loadBuffer(const char * buffer, size_t size)
{
  char * tmp = new char[size + 1];
  memcpy( tmp , buffer, size );
  tmp[size] = '\0';
  return new HttpBody(tmp, size);
}

HttpBody * HttpBody::loadBuffer(char * buffer)
{
  return new HttpBody(buffer, strlen(buffer));
}

HttpBody * HttpBody::loadBuffer(char * buffer, size_t size)
{
  return new HttpBody(buffer, size);
}

size_t HttpBody::size()
{
  return m_size;
}

const char * HttpBody::read()
{
  return m_buffer;
}

void HttpBody::release()
{
  if( m_release == false ) {
    delete[] m_buffer;
    m_buffer  = 0;
    m_size    = 0;
    m_release = true;
  }
}
