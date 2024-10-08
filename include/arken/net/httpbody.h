// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstring>

#ifndef _ARKEN_NET_HTTP_BODY_
#define _ARKEN_NET_HTTP_BODY_

namespace arken {
namespace net {

class HttpBody
{
  private:
  HttpBody(const char *buffer, size_t size);
  HttpBody(char *buffer, size_t size);
  bool   m_release;
  char * m_buffer;
  size_t m_size;

  public:
  ~HttpBody();
  static HttpBody * loadFile(const char *path);
  static HttpBody * loadBuffer(const char *buffer, size_t size);
  static HttpBody * loadBuffer(char *buffer, size_t size);
  static HttpBody * loadBuffer(char * buffer);
  size_t size();
  const char * read();
  void release();
};

} // namespace net
} // namespace arken

#endif
