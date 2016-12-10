#include <QByteArray>
#include <QHash>

class CHttpParser
{

  QByteArray m_data;
  QHash<QByteArray, QByteArray> m_fields;

  const char * m_fragment;
  const char * m_requestPath;
  const char * m_queryString;
  const char * m_requestMethod;
  const char * m_requestUri;
  const char * m_httpVersion;
  const char * m_headerDone;

  public:
  CHttpParser(QByteArray data);
  ~CHttpParser();

  void  setField(const char * fragment, const char * value);
  void  setFragment(const char * fragment);
  void  setQueryString(const char * queryString);
  void  setRequestPath(const char * requestPath);
  void  setRequestMethod(const char * requestMethod);
  void  setRequestUri(const char * requestUri);
  void  setHttpVersion(const char * httpVersion);
  void  setHeaderDone(const char * headerDone);

  const char * fragment();
  const char * queryString();
  const char * requestPath();
  const char * requestMethod();
  const char * requestUri();
  const char * httpVersion();
  const char * headerDone();
  const char * field(const char *);
  char * data();
  QByteArray toJson();

};
