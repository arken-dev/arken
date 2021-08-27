#include <arken/base64.h>
#include <QByteArray>

namespace arken {

char * base64::decode(const char * encoded, size_t * size)
{
  QByteArray text = QByteArray::fromBase64(encoded);
  *size = (size_t) text.size();
  auto result = new char[text.size()+1];
  memcpy(result, text.data(), text.size());
  result[text.size()] = '\0';
  return result;
}

char * base64::encode(const char * plain, int len)
{
  QByteArray ba(plain, len);
  QByteArray encoded = ba.toBase64();
  auto result = new char[encoded.size()+1];
  strncpy(result, encoded.data(), encoded.size());
  result[encoded.size()] = '\0';
  return result;
}

} // namespace arken
