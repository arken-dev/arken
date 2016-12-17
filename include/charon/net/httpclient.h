#ifndef _CHTTP_CLIENT_HEADER_
#define _CHTTP_CLIENT_HEADER_

#include <curl/curl.h>

struct MemoryStruct {
  char * memory;
  size_t size;
};

class CHttpClient
{
  public:
  CHttpClient(const char * url);
  ~CHttpClient();
  void appendHeader(const char * header);
  void setVerbose(bool verbose);
  bool verbose();
  void setBody(const char * body);
  const char * body();
  char * performGet();
  char * performPost();
  char * performPut();
  char * performDelete();

  const char * m_url;
  const char * m_body;
  bool m_verbose;
  curl_slist * m_chunk_list;
  CURL * m_curl;
  MemoryStruct m_chunk;
};

#endif
