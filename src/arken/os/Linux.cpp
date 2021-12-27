#include <arken/base>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/resource.h>

string os::executablePath()
{
  return os::target("/proc/self/exe");
}

size_t os::mem()
{
  struct rusage rusage;
  getrusage( RUSAGE_SELF, &rusage );
  return (size_t)(rusage.ru_maxrss);
}

const char * os::name()
{
  static const char * s_name = "linux";
  return s_name;
}

const char * os::cext()
{
  static const char * s_cext = "so";
  return s_cext;
}

uint64_t os::pid()
{
  return getpid();
}
