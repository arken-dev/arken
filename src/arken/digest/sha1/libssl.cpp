#include <fstream>
#include <arken/base>
#include <arken/digest/sha1.h>

extern "C" {
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
}

namespace arken {
namespace digest {

unsigned char * sha1::bytes(const char * data, int length)
{
  unsigned char * hash = new unsigned char[SHA_DIGEST_LENGTH];
  SHA1(reinterpret_cast<const unsigned char *>(data), length, hash);
  return hash;
}

char * sha1::hash(const char * data, int length)
{
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1(reinterpret_cast<const unsigned char *>(data), length, reinterpret_cast<unsigned char *>(&hash));

  auto result = new char[41];

  for(int i = 0; i < 20; i++)
    sprintf(&result[i*2], "%02x", (unsigned int)hash[i]);

  result[40] = '\0';
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

unsigned char * sha1::assign(const char * path_cert, const char * data, int length)
{

  if(!os::exists(path_cert)) {
    std::cout << path_cert << " invalid path" << std::endl;
    return nullptr;
  }

  int result;
  unsigned char * signature = new unsigned char[256];
  unsigned int siglen = 0;
  unsigned char * bytes = sha1::bytes(data, length);

  string mKey = os::read(path_cert);
  BIO* bo = BIO_new( BIO_s_mem() );
  BIO_write( bo, mKey.data(), mKey.size());
  EVP_PKEY* pkey = 0;
  PEM_read_bio_PrivateKey( bo, &pkey, 0, 0 );

  if(!pkey) {
    std::cout << "invalid certify" << std::endl;
    return nullptr;
  }

  RSA* rsa = EVP_PKEY_get1_RSA( pkey );
  result   = RSA_sign(NID_sha1, bytes, 20, signature, &siglen, rsa);

  BIO_free(bo);
  EVP_PKEY_free(pkey);
  RSA_free(rsa);
  delete[] bytes;

  if( result) {
    return signature;
  } else {
    return nullptr;
  }
}

} // namespace digest
} // namespace arken
