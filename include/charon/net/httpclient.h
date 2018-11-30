// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_NET_HTTP_CLIENT_
#define _CHARON_NET_HTTP_CLIENT_

#include <curl/curl.h>

namespace charon {
namespace net {

class HttpClient
{
  private:
  curl_slist * m_list;
  CURL       * m_curl;
  char       * m_url;
  char       * m_body;
  char       * m_data;
  char       * m_message;
  bool         m_failure;
  uint32_t     m_status;
  uint64_t     m_size;
  char       * perform();
  static
  uint64_t     callback(void *contents, size_t size, size_t nmemb, void *userp);

  public:
  HttpClient(const char * url);
  ~HttpClient();
  void appendHeader(const char * header);
  void setVerbose(bool verbose);
  void setBody(const char * body);
  char * performGet();
  char * performPost();
  char * performPut();
  char * performDelete();
  const
  char * body();
  const
  char * data();
  const
  char * message();
  int    status();
  bool   failure();

};

}
}

#endif
