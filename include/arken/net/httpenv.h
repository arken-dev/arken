// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NET_HTTP_ENV_
#define _ARKEN_NET_HTTP_ENV_

#include <map>
#include <arken/string.h>

namespace arken {
namespace net {

class HttpEnv
{

  using string = arken::string;
  std::map<string, string> m_fields;

  const
  char * m_data;
  size_t m_len;
  string m_fragment;
  string m_requestPath;
  string m_queryString;
  string m_requestMethod;
  string m_requestUri;
  string m_httpVersion;
  string m_headerDone;

  public:
  HttpEnv(const char * data, size_t len);
  ~HttpEnv();

  void  setField(string fragment, string value);
  void  setFragment(const char *at, size_t len);
  void  setQueryString(const char *at, size_t len);
  void  setRequestPath(const char *at, size_t len);
  void  setRequestMethod(const char *at, size_t len);
  void  setRequestUri(const char *at, size_t len);
  void  setHttpVersion(const char *at, size_t len);
  void  setHeaderDone(const char * at, size_t len);

  string fragment();
  string queryString();
  string requestPath();
  string requestMethod();
  string requestUri();
  string httpVersion();
  string headerDone();
  string field(const char *);
  const char * data();
  size_t len();

};

} // namespace net
} // namespace arken

#endif
