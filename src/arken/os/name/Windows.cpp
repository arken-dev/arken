#include <arken/base>

const char * os::name()
{
  static const char * name = "windows";
  return name;
}
