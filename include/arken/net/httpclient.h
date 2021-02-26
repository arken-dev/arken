// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NET_HTTP_CLIENT_
#define _ARKEN_NET_HTTP_CLIENT_

#include <curl/curl.h>
#include <string>
#include <arken/base>

using string = arken::string;

namespace arken {
namespace net {

class HttpClient
{
  private:
  curl_slist * m_list;
  CURL       * m_curl;
  string       m_url;
  string       m_body;
  string       m_data;
  string       m_message;
  bool         m_failure;
  uint32_t     m_status;
  uint64_t     m_size;
  string       perform();
  static
  uint64_t     callback(void *contents, size_t size, size_t nmemb, void *userp);

  public:
  HttpClient(const char * url);
  ~HttpClient();
  void appendHeader(const char * header);
  void setVerbose(bool verbose);
  void setBody(const char * body);
  string performGet();
  string performPost();
  string performPut();
  string performDelete();
  string body();
  string data();
  string message();
  int    status();
  bool   failure();

};

} // namespace net
} // namespace arken

#endif
