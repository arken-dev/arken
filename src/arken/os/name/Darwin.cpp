#include <arken/base>

const char * os::name()
{
  static const char * name = "macos";
  return name;
}
