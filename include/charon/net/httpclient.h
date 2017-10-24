// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHTTP_CLIENT_HEADER_
#define _CHTTP_CLIENT_HEADER_

#include <curl/curl.h>

struct MemoryStruct {
  char * memory;
  size_t size;
};

namespace charon {
namespace net {

class HttpClient
{
  public:
  HttpClient(const char * url);
  ~HttpClient();
  void appendHeader(const char * header);
  void setVerbose(bool verbose);
  bool verbose();
  void setBody(const char * body);
  const char * body();
  char * performGet();
  char * performPost();
  char * performPut();
  char * performDelete();

  char * m_url;
  char * m_body;
  bool m_verbose;
  curl_slist * m_chunk_list;
  CURL * m_curl;
  MemoryStruct m_chunk;
};

}
}

#endif
