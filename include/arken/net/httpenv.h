// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NET_HTTP_ENV_
#define _ARKEN_NET_HTTP_ENV_

#include <map>

namespace arken {
namespace net {

class HttpEnv
{

  std::map<std::string, std::string> m_fields;

  const
  char * m_data;
  char * m_fragment;
  char * m_requestPath;
  char * m_queryString;
  char * m_requestMethod;
  char * m_requestUri;
  char * m_httpVersion;
  char * m_headerDone;
  size_t m_headerDoneLength;
  size_t m_len;

  public:
  HttpEnv(const char * data, size_t len);
  ~HttpEnv();

  void  setField(const char * fragment, const char * value);
  void  setFragment(char * fragment);
  void  setQueryString(char * queryString);
  void  setRequestPath(char * requestPath);
  void  setRequestMethod(char * requestMethod);
  void  setRequestUri(char * requestUri);
  void  setHttpVersion(char * httpVersion);
  void  setHeaderDone(char * headerDone, size_t length);

  const char * fragment();
  const char * queryString();
  const char * requestPath();
  const char * requestMethod();
  const char * requestUri();
  const char * httpVersion();
  const char * headerDone();
  const char * field(const char *);
  const char * data();
  size_t headerDoneLength();

};

} // namespace net
} // namespace arken

#endif
