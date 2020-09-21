#include <arken/base>
#include <QByteArray>

using namespace arken;

char * base64::decode(const char * encoded)
{
  QByteArray text = QByteArray::fromBase64(encoded);
  char * result = new char[text.size()+1];
  strcpy(result, text.data());
  return result;
}

char * base64::encode(const char * plain)
{
  int len = strlen(plain);
  return base64::encode(plain, len);
}

char * base64::encode(const char * plain, int len)
{
  QByteArray ba(plain, len);
  QByteArray encoded = ba.toBase64();
  char * result = new char[encoded.size()+1];
  strcpy(result, encoded.data());
  return result;
}
