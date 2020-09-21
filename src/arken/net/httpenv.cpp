// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

extern "C" {
  #include <http11/http11_parser.h>
}

#include <arken/base>

using namespace arken::net;

static void
http_field_cb(void *data, const char *field, size_t flen, const char *value, size_t vlen)
{
  size_t i;

  char * m_field = new char[flen+1];
  for(i = 0; i < flen; i++) {
    m_field[i] = field[i];
  }
  m_field[i] = '\0';

  char * m_value = new char[vlen+1];
  for(i = 0; i < vlen; i++) {
    m_value[i] = value[i];
  }
  m_value[i] = '\0';

  HttpEnv * p = (HttpEnv *) data;
  p->setField(m_field, m_value);
  delete[] m_field;
  delete[] m_value;
}

static void
on_fragment_cb(void * data, const char * at, size_t length)
{
  size_t i;
  char * tmp = new char[length+1];
  for(i = 0; i < length; i++) {
    tmp[i] = at[i];
  }
  tmp[i] = '\0';

  HttpEnv * p = (HttpEnv *) data;
  p->setFragment(tmp);
}

static void
on_header_done_cb(void *data, const char *at, size_t length)
{
  size_t i;
  char * tmp = new char[length+1];
  for(i = 0; i < length; i++) {
    tmp[i] = at[i];
  }
  tmp[i] = '\0';

  HttpEnv * p = (HttpEnv *) data;
  p->setHeaderDone(tmp, length);
}

static void
on_http_version_cb(void *data, const char *at, size_t length)
{
  size_t i;
  char * tmp = new char[length+1];
  for(i = 0; i < length; i++) {
    tmp[i] = at[i];
  }
  tmp[i] = '\0';

  HttpEnv * p = (HttpEnv *) data;
  p->setHttpVersion(tmp);
}

static void
on_request_uri_cb(void *data, const char *at, size_t length)
{
  size_t i;
  char * tmp = new char[length+1];
  for(i = 0; i < length; i++) {
    tmp[i] = at[i];
  }
  tmp[i] = '\0';

  HttpEnv * p = (HttpEnv *) data;
  p->setRequestUri(tmp);
}

static void
on_request_method_cb(void *data, const char *at, size_t length)
{
  size_t i;
  char * tmp = new char[length+1];
  for(i = 0; i < length; i++) {
    tmp[i] = at[i];
  }
  tmp[i] = '\0';

  HttpEnv * p = (HttpEnv *) data;
  p->setRequestMethod(tmp);
}

static void
on_request_path_cb(void *data, const char *at, size_t length)
{
  size_t i;
  char * tmp = new char[length+1];
  for(i =0; i < length; ++i) {
    tmp[i] = at[i];
  }
  tmp[i] = '\0';

  HttpEnv * p = (HttpEnv *) data;
  p->setRequestPath(tmp);
}

static void
on_query_string_cb(void *data, const char *at, size_t length)
{
  size_t i;
  char * tmp = new char[length+1];
  for(i =0; i < length; ++i) {
    tmp[i] = at[i];
  }
  tmp[i] = '\0';

  HttpEnv * p = (HttpEnv *) data;
  p->setQueryString(tmp);
}

HttpEnv::HttpEnv(const char * data, size_t len)
{

  m_data = data;
  m_len  = len;

  http_parser * parser = (http_parser *) malloc(sizeof(http_parser));
  http_parser_init(parser);

  parser->http_field     = http_field_cb;
  parser->request_method = on_request_method_cb;
  parser->request_uri    = on_request_uri_cb;
  parser->fragment       = on_fragment_cb;
  parser->request_path   = on_request_path_cb;
  parser->query_string   = on_query_string_cb;
  parser->http_version   = on_http_version_cb;
  parser->header_done    = on_header_done_cb;

  parser->data = this;

  m_fragment      = NULL;
  m_requestPath   = NULL;
  m_queryString   = NULL;
  m_requestMethod = NULL;
  m_requestUri    = NULL;
  m_httpVersion   = NULL;
  m_headerDone    = NULL;
  m_headerDoneLength = 0u;

  http_parser_execute(parser, data, len, 0);
  free(parser);
}

HttpEnv::~HttpEnv()
{
  if(m_fragment)
    delete m_fragment;
  if(m_requestPath)
    delete m_requestPath;
  if(m_queryString)
    delete m_queryString;
  if(m_requestMethod)
    delete m_requestMethod;
  if(m_requestUri)
    delete m_requestUri;
  if(m_httpVersion)
    delete m_httpVersion;
  if(m_headerDone)
    delete m_headerDone;
}

void HttpEnv::setField(const char * field, const char * value)
{
  std::string fl(field);
  std::string vl(value);
  m_fields[fl] = vl;
}

void HttpEnv::setFragment(char * fragment)
{
  m_fragment = fragment;
}

void HttpEnv::setHeaderDone(char * headerDone, size_t length)
{
  m_headerDoneLength = length;
  m_headerDone = headerDone;
}

void HttpEnv::setQueryString(char * queryString)
{
  m_queryString = queryString;
}

void HttpEnv::setRequestPath(char * requestPath)
{
  m_requestPath = requestPath;
}

void HttpEnv::setRequestMethod(char * requestMethod)
{
  m_requestMethod = requestMethod;
}

void HttpEnv::setRequestUri(char * requestUri)
{
  m_requestUri = requestUri;
}

void HttpEnv::setHttpVersion(char * httpVersion)
{
  m_httpVersion = httpVersion;
}

const char * HttpEnv::queryString()
{
  return m_queryString;
}

const char * HttpEnv::requestPath()
{
  return m_requestPath;
}

const char * HttpEnv::requestMethod()
{
  return m_requestMethod;
}

const char * HttpEnv::requestUri()
{
  return m_requestUri;
}

const char * HttpEnv::httpVersion()
{
  return m_httpVersion;
}

const char * HttpEnv::headerDone()
{
  return m_headerDone;
}

size_t HttpEnv::headerDoneLength()
{
  return m_headerDoneLength;
}


const char * HttpEnv::fragment()
{
  return m_fragment;
}

const char * HttpEnv::field(const char * field)
{
  std::string str(field);
  return m_fields[str].c_str();
}

const char * HttpEnv::data()
{
  return m_data;
}
