/**
 * Using libcurl for http request
 *  https://curl.haxx.se/libcurl/c/example.html
 */
namespace charon {
  namespace http {
    char * read(const char * url);
    char * get(const char * url);
    char * post(const char * url);
    char * put(const char * url);
  }
}
