#include <charon/base>
extern "C" {
#include <openssl/md5.h>
#include <openssl/sha.h>
}

using namespace charon;

char * digest::md5(const char * hash)
{
  return digest::md5(hash, strlen(hash));
}

char * digest::md5(const char * string, int length)
{
  unsigned char digest[MD5_DIGEST_LENGTH];

  MD5((unsigned char*)string, length, (unsigned char*)&digest);

  char * result = new char[33];

  for(int i = 0; i < 16; i++)
    sprintf(&result[i*2], "%02x", (unsigned int)digest[i]);

  return result;
}

char * digest::sha1(const char * hash)
{
  return digest::sha1(hash, strlen(hash));
}

char * digest::sha1(const char * data, int length)
{
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1((unsigned char *)data, length, (unsigned char*)&hash);

  char * result = new char[41];

  for(int i = 0; i < 20; i++)
    sprintf(&result[i*2], "%02x", (unsigned int)hash[i]);

  return result;
}
