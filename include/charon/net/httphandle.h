#ifndef CHARON_HTTP_HANDLE
#define CHARON_HTTP_HANDLE
namespace charon {
namespace net {
  class HttpHandle
  {
    public:
    static char * sync(const char * data, int size, int * num);
    static const char * status(int code);
  };
}
}
#endif //CHARON_HTTP_HANDLE
