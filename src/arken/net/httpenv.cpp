// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

extern "C" {
  #include <http11/http11_parser.h>
}

#include <memory>
#include <arken/net/httpenv.h>

namespace arken {
namespace net {

static void
http_field_cb(void *data, const char *field, size_t flen, const char *value, size_t vlen)
{
  auto p = static_cast<HttpEnv *> (data);
  p->setField(string(field, flen), string(value, vlen));
}

static void
on_fragment_cb(void * data, const char * at, size_t len)
{
  auto p = static_cast<HttpEnv *> (data);
  p->setFragment(at, len);
}

static void
on_header_done_cb(void *data, const char *at, size_t len)
{
  auto p = static_cast<HttpEnv *> (data);
  p->setHeaderDone(at, len);
}

static void
on_http_version_cb(void *data, const char *at, size_t len)
{
  auto p = static_cast<HttpEnv *> (data);
  p->setHttpVersion(at, len);
}

static void
on_request_uri_cb(void *data, const char *at, size_t len)
{
  auto p = static_cast<HttpEnv *> (data);
  p->setRequestUri(at, len);
}

static void
on_request_method_cb(void *data, const char *at, size_t len)
{
  auto p = static_cast<HttpEnv *> (data);
  p->setRequestMethod(at, len);
}

static void
on_request_path_cb(void *data, const char *value, size_t len)
{
  auto p = static_cast<HttpEnv *> (data);
  p->setRequestPath(value, len);
}

static void
on_query_string_cb(void *data, const char *value, size_t len)
{
  auto p = static_cast<HttpEnv *> (data);
  p->setQueryString(value, len);
}

HttpEnv::HttpEnv(const char * data, size_t len)
{

  m_data = data;
  m_len  = len;

  std::unique_ptr<http_parser> parser(new http_parser);
  http_parser_init(parser.get());

  parser->http_field     = http_field_cb;
  parser->request_method = on_request_method_cb;
  parser->request_uri    = on_request_uri_cb;
  parser->fragment       = on_fragment_cb;
  parser->request_path   = on_request_path_cb;
  parser->query_string   = on_query_string_cb;
  parser->http_version   = on_http_version_cb;
  parser->header_done    = on_header_done_cb;

  parser->data = this;

  http_parser_execute(parser.get(), data, len, 0);
}

HttpEnv::~HttpEnv()
{
}

void HttpEnv::setField(string field, string value)
{
  m_fields[field] = value;
}

void HttpEnv::setFragment(const char * at, size_t len)
{
  m_fragment = string(at, len);
}

void HttpEnv::setHeaderDone(const char * at, size_t len)
{
  m_headerDone = string(at, len);
}

void HttpEnv::setQueryString(const char * at, size_t len)
{
  m_queryString = string(at, len);
}

void HttpEnv::setRequestPath(const char * at, size_t len)
{
  m_requestPath = string(at, len);
}

void HttpEnv::setRequestMethod(const char * at, size_t len)
{
  m_requestMethod = string(at, len);
}

void HttpEnv::setRequestUri(const char * at, size_t len)
{
  m_requestUri = string(at, len);
}

void HttpEnv::setHttpVersion(const char * at, size_t len)
{
  m_httpVersion = string(at, len);
}

string HttpEnv::queryString()
{
  return m_queryString;
}

string HttpEnv::requestPath()
{
  return m_requestPath;
}

string HttpEnv::requestMethod()
{
  return m_requestMethod;
}

string HttpEnv::requestUri()
{
  return m_requestUri;
}

string HttpEnv::httpVersion()
{
  return m_httpVersion;
}

string HttpEnv::headerDone()
{
  return m_headerDone;
}

string HttpEnv::fragment()
{
  return m_fragment;
}

string HttpEnv::field(const char * key)
{
  return m_fields[key];
}

const char * HttpEnv::data()
{
  return m_data;
}

size_t HttpEnv::len()
{
  return m_len;
}

} // namespace net
} // namespace arken
