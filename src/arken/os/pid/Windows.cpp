#include <arken/base>
#include <process.h>

uint64_t os::pid()
{
  return _getpid();
}
