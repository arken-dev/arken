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


uint64_t HttpClient::callback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  HttpClient * client = (HttpClient *)userp;

  client->m_data = (char *) realloc(client->m_data, client->m_size + realsize + 1);
  // out of memory
  if(client->m_data == NULL) {
    client->m_failure = true;
    client->m_message = (char *) "not enough memory (realloc returned NULL)";
    return 0;
  }

  memcpy(&(client->m_data[client->m_size]), contents, realsize);
  client->m_size += realsize;
  client->m_data[client->m_size] = 0;

  return realsize;
}

HttpClient::HttpClient(const char * url)
{
  //url
  m_url = new char[strlen(url) + 1];
  strcpy(m_url, url);

  m_body    = NULL;
  m_data    = NULL;
  m_message = NULL;
  m_size    = 0;
  m_list    = 0;
  m_failure = false;

  // init globlal
  //curl_global_init(CURL_GLOBAL_ALL);

  // init the curl session
  m_curl = curl_easy_init();

  // url
  curl_easy_setopt(m_curl, CURLOPT_URL, url);

  // example.com is redirected, so we tell libcurl to follow redirection
  curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);

  // send all data to this function
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, HttpClient::callback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)this);

  // headers
  curl_easy_setopt(m_curl, CURLOPT_HEADER, 1);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
}

HttpClient::~HttpClient()
{
  // cleanup curl stuff
  curl_easy_cleanup(m_curl);


  // we're done with libcurl, so clean it up
  curl_global_cleanup();

  // free memory
  delete[] m_url;
  if( m_data )    delete[] m_data;
  if( m_body )    delete[] m_body;
  if( m_message ) delete[] m_message;

}

void HttpClient::appendHeader(const char * header)
{
   m_list = curl_slist_append(m_list, header);
}

void HttpClient::setVerbose(bool verbose)
{
  if( verbose ) {
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
  } else {
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0L);
  }
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
  return perform();
}

char * HttpClient::performPost()
{

  /* POST */
  curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(m_curl, CURLOPT_POST, 1);
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_body);
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, strlen(m_body));

  return perform();
}

char * HttpClient::performPut()
{

  /* PUT */
  curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(m_curl, CURLOPT_POST, 1);
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_body);
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, strlen(m_body));

  return perform();
}

char * HttpClient::performDelete()
{

  /* DELETE */
  curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "DELETE");

  return perform();
}

int HttpClient::status()
{
  return m_status;
}

const char * HttpClient::data()
{
  return m_data;
}

const char * HttpClient::message()
{
  return m_message;
}

bool HttpClient::failure()
{
  return m_failure;
}

char * HttpClient::perform()
{
  char    * body;
  int       index;
  CURLcode  res;

  // set our custom set of headers
  curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_list);

  // perform
  res = curl_easy_perform(m_curl);

  // out of memory
  if ( m_failure ) {
    return new char[1]();
  }

  // check for errors
  if( res != CURLE_OK ) {
    m_failure = true;
    const char * message = curl_easy_strerror(res);
    m_message = new char[strlen(message)+1];
    strcpy(m_message, message);
    return new char[1]();
  }

  if( m_size ) {

    // parse status
    index = string::indexOf(m_data, " ");
    if( index > -1 ) {
      m_status = atoi(string::mid(m_data, index + 1, index + 4));
    } else {
      m_status = 0;
    }

    //parse body
    index = string::indexOf(m_data, "\r\n\r\n");
    if( index > 0 ) {
      body = string::mid(m_data, index+4, -1);
    } else {
      body = new char[1]();
    }
  } else {
    m_status = 0;
    body = new char[1]();
  }

  return body;
}
