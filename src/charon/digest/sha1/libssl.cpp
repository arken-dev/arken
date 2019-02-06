#include <charon/base>
extern "C" {
#include <openssl/sha.h>
}

using namespace charon::digest;

char * sha1::hash(const char * hash)
{
  return sha1::hash(hash, strlen(hash));
}

char * sha1::hash(const char * data, int length)
{
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1((unsigned char *)data, length, (unsigned char*)&hash);

  char * result = new char[41];

  for(int i = 0; i < 20; i++)
    sprintf(&result[i*2], "%02x", (unsigned int)hash[i]);

  return result;
}
