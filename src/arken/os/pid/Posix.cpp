#include <arken/base>
#include <unistd.h>

uint64_t os::pid()
{
  return getpid();
}
