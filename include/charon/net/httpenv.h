// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef __CHARON_NET_HTTP_ENV__
#define __CHARON_NET_HTTP_ENV__

#include <QByteArray>
#include <QHash>

namespace charon {
namespace net {

class HttpEnv
{

  QByteArray m_data;
  QHash<QByteArray, QByteArray> m_fields;

  const char * m_fragment;
  const char * m_requestPath;
  const char * m_queryString;
  const char * m_requestMethod;
  const char * m_requestUri;
  const char * m_httpVersion;
  const char * m_headerDone;
  size_t m_headerDoneLength;

  public:
  HttpEnv(QByteArray data);
  ~HttpEnv();

  void  setField(const char * fragment, const char * value);
  void  setFragment(const char * fragment);
  void  setQueryString(const char * queryString);
  void  setRequestPath(const char * requestPath);
  void  setRequestMethod(const char * requestMethod);
  void  setRequestUri(const char * requestUri);
  void  setHttpVersion(const char * httpVersion);
  void  setHeaderDone(const char * headerDone, size_t length);

  const char * fragment();
  const char * queryString();
  const char * requestPath();
  const char * requestMethod();
  const char * requestUri();
  const char * httpVersion();
  const char * headerDone();
  size_t headerDoneLength();
  const char * field(const char *);
  char * data();
  QByteArray toJson();

};

}
}

#endif
