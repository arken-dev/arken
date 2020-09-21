#include <charon/base>

const char * os::name()
{
  static const char * name = "linux";
  return name;
}
