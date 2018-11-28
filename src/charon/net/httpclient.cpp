// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <curl/curl.h>
#include <cstdlib>
#include <cstring>
#include <charon/base>
#include <iostream>

using charon::string;
using namespace charon::net;

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = (char *) realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    // out of memory!
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

HttpClient::HttpClient(const char * url)
{
  size_t size;
  size = strlen(url);
  m_url = new char[size + 1];
  strcpy(m_url, url);
  m_url[size] = '\0';

  m_verbose = false;
  m_body = new char[1]();
  m_urlRedirect = 0;

  m_chunk.memory = (char *) malloc(1);  // will be grown as needed by the realloc above
  m_chunk.memory[0] = '\0';
  m_chunk.size = 0;    // no data at this point
  m_chunk_list = NULL;

  curl_global_init(CURL_GLOBAL_ALL);

  // init the curl session
  m_curl = curl_easy_init();

  // url
  curl_easy_setopt(m_curl, CURLOPT_URL, url);
}

HttpClient::~HttpClient()
{
  // cleanup curl stuff
  curl_easy_cleanup(m_curl);

  // free memory result
  free(m_chunk.memory);


  // we're done with libcurl, so clean it up
  curl_global_cleanup();

  delete[] m_url;
  delete[] m_body;
  if( m_urlRedirect != 0 ) {
    delete[] m_urlRedirect;
  }
}

void HttpClient::appendHeader(const char * header)
{
   m_chunk_list = curl_slist_append(m_chunk_list, header);
}

void HttpClient::setVerbose(bool verbose)
{
  m_verbose = verbose;
  curl_easy_setopt(m_curl, CURLOPT_VERBOSE, verbose);//1L);
}

const char * HttpClient::urlRedirect()
{
  return m_urlRedirect;
}

bool HttpClient::verbose()
{
  return m_verbose;
}

void HttpClient::setBody(const char * body)
{
  size_t size;
  size = strlen(body);
  m_body = new char[size + 1];
  strcpy(m_body, body);
  m_body[size] = '\0';
}

const char * HttpClient::body()
{
  return m_body;
}

char * HttpClient::performGet()
{
  CURLcode res;

  /* set our custom set of headers */
  res = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_chunk_list);

  // send all data to this function
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)& m_chunk);
  curl_easy_setopt(m_curl, CURLOPT_HEADER, 1);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // get it!
  res = curl_easy_perform(m_curl);

  // check for errors
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }

  m_chunk.memory[m_chunk.size] = '\0';

  return perform(m_chunk.memory);
}

char * HttpClient::performPost()
{
  CURLcode res;

  /* set our custom set of headers */
  res = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_chunk_list);

  /* POST */
  curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(m_curl, CURLOPT_POST, 1);
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_body);
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, strlen(m_body));

  // send all data to this function
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)& m_chunk);
  curl_easy_setopt(m_curl, CURLOPT_HEADER, 1);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // post it!
  res = curl_easy_perform(m_curl);

  // check for errors
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }

  m_chunk.memory[m_chunk.size] = '\0';

  return perform(m_chunk.memory);
}

char * HttpClient::performPut()
{
  CURLcode res;

  /* set our custom set of headers */
  res = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_chunk_list);

  /* PUT */
  curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(m_curl, CURLOPT_POST, 1);
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_body);
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, strlen(m_body));

  // send all data to this function
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)& m_chunk);
  curl_easy_setopt(m_curl, CURLOPT_HEADER, 1);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // put it!
  res = curl_easy_perform(m_curl);

  // check for errors
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }

  m_chunk.memory[m_chunk.size] = '\0';

  parseStatus(m_chunk.memory);

  return 0;
}

char * HttpClient::performDelete()
{
  CURLcode res;

  /* set our custom set of headers */
  res = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_chunk_list);

  /* DELETE */
  curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "DELETE");

  // send all data to this function
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)& m_chunk);
  curl_easy_setopt(m_curl, CURLOPT_HEADER, 1);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // delete it!
  res = curl_easy_perform(m_curl);

  // check for errors
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }

  m_chunk.memory[m_chunk.size] = '\0';

  return perform(m_chunk.memory);
}

int HttpClient::status()
{
  return m_status;
}

void HttpClient::parseStatus(char * memory)
{
  int indexStatus = string::indexOf(memory, " ");
  m_status = atoi(string::mid(memory, indexStatus + 1, indexStatus + 4 ));
}

char * HttpClient::perform(char * memory)
{
  int      index  = string::indexOf(memory, "\r\n\r\n");
  char * headers  = string::mid(memory, 0, index);

  parseStatus(memory);

  if( string::contains(headers, "\r\nLocation") ) {
    index = string::indexOf(headers, "Location:");
    char * tmp = string::mid(headers, index+9, string::indexOf(headers, "\r\n", index+9) - (index+9));
    char * url = string::trimmed(tmp);
    m_urlRedirect = url;
    if( m_verbose ) {
      std::cout << "redirect: " << url << "\n";
    }
    HttpClient client = HttpClient(url);
    client.setVerbose(m_verbose);
    delete[] tmp;
    delete[] headers;
    return client.performGet();
  } else {
    delete[] headers;
    return string::mid(memory, index+4, -1);
  }
}
