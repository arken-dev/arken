namespace charon
{

class digest {
  public:
  static char * md4(const char * hash);
  static char * md4(const char * hash, int length);
  static char * md5(const char * hash);
  static char * md5(const char * hash, int length);
  static char * sha1(const char * hash);
  static char * sha1(const char * hash, int length);
  static char * sha224(const char * hash);
  static char * sha224(const char * hash, int length);
  static char * sha256(const char * hash);
  static char * sha256(const char * hash, int length);
  static char * sha384(const char * hash);
  static char * sha384(const char * hash, int length);
  static char * sha512(const char * hash);
  static char * sha512(const char * hash, int length);
};

}
