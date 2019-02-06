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

char * sha1::file(const char * path)
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
