#include <fstream>
#include <arken/base>
#include <arken/digest/md5.h>

extern "C" {
#include <openssl/md5.h>
}

namespace arken {
namespace digest {

char * md5::hash(const char * string, int length)
{
  unsigned char digest[MD5_DIGEST_LENGTH];

  MD5(reinterpret_cast<const unsigned char*>(string), length, reinterpret_cast<unsigned char*>(&digest));

  auto result = new char[33];

  for(int i = 0; i < 16; i++)
    sprintf(&result[i*2], "%02x", (unsigned int)digest[i]);

  result[32] = '\0';
  return result;
}

char * md5::file(const char * path)
{
  char * buffer;
  std::ifstream file;
  int length;
  file.open(path);
  file.seekg(0, std::ios::end);
  length = file.tellg();
  file.seekg(0, std::ios::beg);
  buffer = new char[length];
  file.read(buffer, length);
  file.close();
  char * result = hash(buffer, length);
  delete[] buffer;

  return result;
}

} // namespace digest
} // namespace arken
