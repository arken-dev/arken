// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <curl/curl.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <arken/net/httpclient.h>
#include <atomic>
#include <mutex>

static uint32_t   global_count{0};
static std::mutex global_mutex;

namespace arken {
namespace net {

uint64_t HttpClient::callback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  auto client = static_cast<HttpClient *>(userp);
  client->m_data.append(static_cast<const char *>(contents), realsize);
  return realsize;
}

HttpClient::HttpClient(const char * url)
{
  m_url     = url;
  m_status  = 0;
  m_size    = 0;
  m_failure = false;
  m_verbose = false;
}

HttpClient::~HttpClient() = default;

void HttpClient::appendHeader(const char * header)
{
  m_headers.push_back(header);
}

void HttpClient::setVerbose(bool verbose)
{
  m_verbose = verbose;
}

void HttpClient::setBody(const char * body)
{
  m_body = body;
}

string HttpClient::body()
{
  return m_body;
}

string HttpClient::performGet()
{
  return perform();
}

string HttpClient::performPost()
{
  return perform("POST");
}

string HttpClient::performPut()
{
  return perform("PUT");
}

string HttpClient::performDelete()
{
  return perform("DELETE");
}

int HttpClient::status()
{
  return m_status;
}

string HttpClient::data()
{
  return m_data;
}

string HttpClient::message()
{
  return m_message;
}

bool HttpClient::failure()
{
  return m_failure;
}

string HttpClient::perform(string method)
{
  curl_slist * list = nullptr;
  CURL       * curl;
  int          index;
  CURLcode     res;

  // init globlal
  global_mutex.lock();
  if( global_count == 0 ) {
    std::cout << "curl_global_init" << std::endl;
    curl_global_init(CURL_GLOBAL_ALL);
  }
  global_count++;
  global_mutex.unlock();

  // init the curl session
  curl = curl_easy_init();

  // url
  curl_easy_setopt(curl, CURLOPT_URL, m_url.data());

  // verbose
  if( m_verbose ) {
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  } else {
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
  }

  // example.com is redirected, so we tell libcurl to follow redirection
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);

  // send all data to this function
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::callback);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)this);

  // headers
  curl_easy_setopt(curl, CURLOPT_HEADER, 1);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // https://curl.haxx.se/docs/sslcerts.html
  // Tell libcurl to not verify the peer. With libcurl you disable this with
  // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
  // With the curl command line tool, you disable this with -k/--insecure.
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

  for(size_t i=0; i < m_headers.size(); i++) {
    list = curl_slist_append(list, m_headers[i].data());
  }

  // set our custom set of headers
  if( list ) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
  }

  // POST PUT DELETE
  if( !method.empty() ) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.data());
  }

  // POST PUT
  if( method.equals("POST") || method.equals("PUT") ) {
    //curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_body.data());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, m_body.size());
  }

  // perform
  res = curl_easy_perform(curl);

  // cleanup curl stuff
  curl_easy_cleanup(curl);

  /* Free the list */
  if( list ) {
    curl_slist_free_all(list);
  }

  // we're done with libcurl, so clean it up
  global_mutex.lock();
  global_count--;
  if( global_count == 0 ) {
    std::cout << "curl_global_cleanup" << std::endl;
    curl_global_cleanup();
  }
  global_mutex.unlock();

  // out of memory
  if ( m_failure ) {
    return {};
  }

  // check for errors
  if( res != CURLE_OK ) {
    m_failure = true;
    m_message = curl_easy_strerror(res);
    return {};
  }

  if( !m_data.empty() ) {
    // parse status
    index = m_data.lastIndexOf("HTTP");
    index = m_data.indexOf(" ", index);
    if( index > -1 ) {
      //m_status = m_data.mid(index+1, index+4).atoi();
      string status = m_data.mid(index + 1, index + 4);
      // TODO atoi in arken:string
      m_status = atoi(status);
    } else {
      m_status = 0;
    }

    // parse body TODO refactory return {}
    index = m_data.lastIndexOf("\r\n\r\n");
    if( index > 0 ) {
      index += 4;
      size_t size = (m_data.size()-index);
      if( size > 0 ) {
        return m_data.mid(index, size);
      } else {
        return {};
      }
    } else {
      return {};
    }
  } else {
    m_status = 0;
    return {};
  }
}

} // namespace net
} // namespace arken
