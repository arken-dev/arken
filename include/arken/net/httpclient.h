// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NET_HTTP_CLIENT_
#define _ARKEN_NET_HTTP_CLIENT_

#include <arken/base>
#include <vector>

namespace arken {
namespace net {

class HttpClient
{
  using vector = std::vector<string>;

  private:
  string       m_url;
  string       m_body;
  string       m_data;
  string       m_message;
  string       m_cert;
  string       m_certKey;
  bool         m_verbose;
  bool         m_formdata;
  bool         m_sslVerifyPeer;
  long         m_sslVerifyHost;
  long         m_sslVersion;
  long         m_useSsl;
  bool         m_failure;
  uint32_t     m_status;
  vector       m_headers;
  string       perform(string method = "");
  static
  uint64_t     callback(void *contents, size_t size, size_t nmemb, void *userp);

  public:
  HttpClient(const char * url);
  ~HttpClient();
  void appendHeader(const char * header);
  void setVerbose(bool verbose);
  void setBody(string body);
  void setCert(string cert);
  void setCertKey(string certKey);
  void setSslVerifyPeer(bool sslVerifyPeer);
  void setSslVerifyHost(long sslVerifyHost);
  void setSslVersion(long sslVersion);
  void setUseSsl(long useSsl);
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