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
  bool         m_failure;
  bool         m_verbose;
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
  /*
  void setPathCert(string pathCert);
../fusion/rails/lib/fusion/gnre_methods.rb:    client.cert            = File.expand_path(self.path_cert)
  void setCert(string path_cert);
../fusion/rails/lib/fusion/gnre_methods.rb:    client.cert_key        = File.expand_path(self.path_key)
  void setCertKey(string path_key);
../fusion/rails/lib/fusion/gnre_methods.rb:    client.ssl_verify_peer = false
  void setSslVerifyPeer(bool sslVerifyPeer);
../fusion/rails/lib/fusion/gnre_methods.rb:    client.ssl_verify_host = 2
  void setSslVerifyHost();
../fusion/rails/lib/fusion/gnre_methods.rb:    client.ssl_version     = 0
  void setSslVersion(int version);
../fusion/rails/lib/fusion/gnre_methods.rb:    client.use_ssl         = 3
  void setUseSsl(int version);
  */
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
