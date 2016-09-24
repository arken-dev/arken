namespace oberon {
  namespace digest {
    char * md4(const char * hash);
    char * md4(const char * hash, int length);
    char * md5(const char * hash);
    char * md5(const char * hash, int length);
    char * sha1(const char * hash);
    char * sha1(const char * hash, int length);
    char * sha224(const char * hash);
    char * sha224(const char * hash, int length);
    char * sha256(const char * hash);
    char * sha256(const char * hash, int length);
    char * sha384(const char * hash);
    char * sha384(const char * hash, int length);
    char * sha512(const char * hash);
    char * sha512(const char * hash, int length);
  }
}
