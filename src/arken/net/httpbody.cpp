// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/net/httpbody.h>

namespace arken {
namespace net {

HttpBody::HttpBody(const char * buffer, size_t size)
{
  m_release = false;
  m_buffer  = new char[size+1];
  m_size    = size;
  memcpy( m_buffer, buffer, size );
  m_buffer[size] = '\0';
}

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
  string buffer = os::read(path);
  return new HttpBody(buffer.release(), buffer.size());
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

} // namespace net
} // namespace arken
