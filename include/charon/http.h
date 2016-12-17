/**
 * Using libcurl for http request
 *  https://curl.haxx.se/libcurl/c/example.html
 */
namespace charon
{

class http {
  static char * read(const char * url);
  static char * get(const char * url);
  static char * post(const char * url);
  static char * put(const char * url);
};

}
