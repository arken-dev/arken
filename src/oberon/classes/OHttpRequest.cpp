extern "C" {
  #include <http11/http11_parser.h>
}

#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <OHttpRequest>

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

  OHttpRequest * p = (OHttpRequest *) data;
  p->setField(m_field, m_value);
  delete[] m_field;
  delete[] m_value;
}

static void
on_fragment_cb(void * data, const char * at, size_t length)
{
  std::cout << "FRAGMENT...";
  size_t i;
  char * tmp = new char[length+1];
  for(i = 0; i < length; i++) {
    tmp[i] = at[i];
  }
  tmp[i] = '\0';

  OHttpRequest * p = (OHttpRequest *) data;
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

  OHttpRequest * p = (OHttpRequest *) data;
  p->setHeaderDone(tmp);
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

  OHttpRequest * p = (OHttpRequest *) data;
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

  OHttpRequest * p = (OHttpRequest *) data;
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

  OHttpRequest * p = (OHttpRequest *) data;
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

  OHttpRequest * p = (OHttpRequest *) data;
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

  OHttpRequest * p = (OHttpRequest *) data;
  p->setQueryString(tmp);
}

OHttpRequest::OHttpRequest(QByteArray data)
{
  m_data = data;
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
  //free(http_parser);
  m_fragment      = NULL;
  m_requestPath   = NULL;
  m_queryString   = NULL;
  m_requestMethod = NULL;
  m_requestUri    = NULL;
  m_httpVersion   = NULL;
  m_headerDone    = NULL;
  http_parser_execute(parser, m_data.data(), m_data.size(), 0);
  free(parser);
}

OHttpRequest::~OHttpRequest()
{
  if(m_fragment != NULL)
    delete m_fragment;
  if(m_requestPath != NULL)
    delete m_requestPath;
  if(m_queryString != NULL)
    delete m_queryString;
  if(m_requestMethod != NULL)
    delete m_requestMethod;
  if(m_requestUri != NULL)
    delete m_requestUri;
  if(m_httpVersion != NULL)
    delete m_httpVersion;
  if(m_headerDone != NULL)
    delete m_headerDone;
}

void OHttpRequest::setField(const char * field, const char * value)
{
  m_fields.insert(QByteArray(field), value);
}

void OHttpRequest::setFragment(const char * fragment)
{
  m_fragment = fragment;
}

void OHttpRequest::setHeaderDone(const char * headerDone)
{
  m_headerDone = headerDone;
}

void OHttpRequest::setQueryString(const char * queryString)
{
  m_queryString = queryString;
}

void OHttpRequest::setRequestPath(const char * requestPath)
{
  m_requestPath = requestPath;
}

void OHttpRequest::setRequestMethod(const char * requestMethod)
{
  m_requestMethod = requestMethod;
}

void OHttpRequest::setRequestUri(const char * requestUri)
{
  m_requestUri = requestUri;
}

void OHttpRequest::setHttpVersion(const char * httpVersion)
{
  m_httpVersion = httpVersion;
}

const char * OHttpRequest::queryString()
{
  return m_queryString;
}

const char * OHttpRequest::requestPath()
{
  return m_requestPath;
}

const char * OHttpRequest::requestMethod()
{
  return m_requestMethod;
}

const char * OHttpRequest::requestUri()
{
  return m_requestUri;
}

const char * OHttpRequest::httpVersion()
{
  return m_httpVersion;
}

const char * OHttpRequest::headerDone()
{
  return m_headerDone;
}

const char * OHttpRequest::fragment()
{
  return m_fragment;
}

const char * OHttpRequest::field(const char * field)
{
  return m_fields.value(QByteArray(field));
}

char * OHttpRequest::data()
{
  return m_data.data();
}

QByteArray OHttpRequest::toJson()
{
  QJsonObject json;
  json["fragment"]      = m_fragment;
  json["requestPath"]   = m_requestPath;
  json["queryString"]   = m_queryString;
  json["requestMethod"] = m_requestMethod;
  json["requestUri"]    = m_requestUri;
  json["httpVersion"]   = m_httpVersion;
  json["headerDone"]    = m_headerDone;

  QList<QByteArray> list = m_fields.keys();
  for (int i = 0; i < list.size(); ++i) {
    QByteArray key = list.at(i);
    json[key] = m_fields[key].data();
  }
  QJsonDocument document(json);
  return document.toJson(); 
}
