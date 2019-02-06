#include <charon/base>
extern "C" {
#include <openssl/md5.h>
}

using namespace charon::digest;

char * md5::hash(const char * hash)
{
  return md5::hash(hash, strlen(hash));
}

char * md5::hash(const char * string, int length)
{
  unsigned char digest[MD5_DIGEST_LENGTH];

  MD5((unsigned char*)string, length, (unsigned char*)&digest);

  char * result = new char[33];

  for(int i = 0; i < 16; i++)
    sprintf(&result[i*2], "%02x", (unsigned int)digest[i]);

  return result;
}
