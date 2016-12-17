namespace charon
{

class base64 {
  public:
  static char * decode(const char * data);
  static char * encode(const char * data, int len);
  static char * encode(const char * data);
};

}
