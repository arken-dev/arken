// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <curl/curl.h>
#include <charon/base>
#include <iostream>

using namespace charon;

struct MemoryStructHttp {
  char * memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStructHttp *mem = (struct MemoryStructHttp *)userp;

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

char * http::read(const char * url)
{
  CURL *curl_handle;
  CURLcode res;

  struct MemoryStructHttp chunk;

  chunk.memory = (char *) malloc(1);  // will be grown as needed by the realloc above
  chunk.memory[0] = '\0';
  chunk.size = 0;    // no data at this point

  curl_global_init(CURL_GLOBAL_ALL);

  // init the curl session
  curl_handle = curl_easy_init();

  // specify URL to get
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);

  // send all data to this function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // get it!
  res = curl_easy_perform(curl_handle);

  // check for errors
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }

  // cleanup curl stuff
  curl_easy_cleanup(curl_handle);

  //free(chunk.memory);

  // we're done with libcurl, so clean it up
  curl_global_cleanup();

  return chunk.memory;
}

char * http::get(const char * url)
{
  CURL *curl_handle;
  CURLcode res;

  struct MemoryStructHttp chunk;

  chunk.memory = (char *) malloc(1);  // will be grown as needed by the realloc above
  chunk.size = 0;    // no data at this point

  curl_global_init(CURL_GLOBAL_ALL);

  // init the curl session
  curl_handle = curl_easy_init();

  // specify URL to get
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);

  // send all data to this function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // get it!
  res = curl_easy_perform(curl_handle);

  // check for errors
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }

  // cleanup curl stuff
  curl_easy_cleanup(curl_handle);

  // we're done with libcurl, so clean it up
  curl_global_cleanup();

  return chunk.memory;
}

char * http::post(const char * url)
{
  /*
    URL DE EXEMPLO: http://www.objectdata.com.br/produto/123?descricao=cascola
    URL EXAMPLE: http://www.objectdata.com.br/produto/123?descricao=cascola

    TUDO QUE TIVER ANTES DO ?, VAI FORMAR A URL DA REQUISIÇÃO
    ALL YOU HAVE BEFORE ?, GO TO FORM A URL OF REQUEST

    TUDO QUE TIVER APOS DO ?, VAI FORMAR O CORPO DA REQUISIÇÃO
    ALL YOU HAVE AFTER ?, WILL BUILD THE BODY OF THE REQUEST
  */
  int pos = string::indexOf(url, "?");
  const char * body = string::mid(url, pos+1, strlen(url));
  const char * url_final = string::left(url, pos);

  CURL *curl_handle;
  CURLcode res;

  struct MemoryStructHttp chunk;

  chunk.memory = (char *) malloc(1);  // will be grown as needed by the realloc above
  chunk.size = 0;    // no data at this point

  curl_global_init(CURL_GLOBAL_ALL);

  // init the curl session
  curl_handle = curl_easy_init();

  curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, body);
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, strlen(body));

  // specify URL
  curl_easy_setopt(curl_handle, CURLOPT_URL, url_final);

  // send all data to this function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  //-------------------------------------------------

  // post it!
  res = curl_easy_perform(curl_handle);

  // check for errors
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }

  // cleanup curl stuff
  curl_easy_cleanup(curl_handle);

  // we're done with libcurl, so clean it up
  curl_global_cleanup();
  chunk.memory[chunk.size] = '\0';
  return chunk.memory;
}

char * http::put(const char * url)
{
  /*
    URL DE EXEMPLO: http://www.objectdata.com.br/produto/123?descricao=cascola
    URL EXAMPLE: http://www.objectdata.com.br/produto/123?descricao=cascola

    TUDO QUE TIVER ANTES DO ?, VAI FORMAR A URL DA REQUISIÇÃO
    ALL YOU HAVE BEFORE ?, GO TO FORM A URL OF REQUEST

    TUDO QUE TIVER APOS DO ?, VAI FORMAR O CORPO DA REQUISIÇÃO
    ALL YOU HAVE AFTER ?, WILL BUILD THE BODY OF THE REQUEST
  */
  int pos = string::indexOf(url, "?");
  const char * body = string::mid(url, pos+1, strlen(url));
  const char * url_final = string::left(url, pos);

  CURL *curl_handle;
  CURLcode res;

  struct MemoryStructHttp chunk;

  chunk.memory = (char *) malloc(1);  // will be grown as needed by the realloc above
  chunk.size = 0;    // no data at this point

  curl_global_init(CURL_GLOBAL_ALL);

  // init the curl session
  curl_handle = curl_easy_init();

  curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, body);
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, strlen(body));

  // specify URL
  curl_easy_setopt(curl_handle, CURLOPT_URL, url_final);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // send all data to this function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  size_t io_len;

  res = curl_easy_perform(curl_handle);
  res = curl_easy_send(curl_handle, body, strlen(body), &io_len);

  // check for errors
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }
  // cleanup curl stuff
  curl_easy_cleanup(curl_handle);

  // we're done with libcurl, so clean it up
  curl_global_cleanup();
  chunk.memory[chunk.size] = '\0';
  return chunk.memory;
}
