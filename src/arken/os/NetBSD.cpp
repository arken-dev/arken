#include <sys/resource.h>
#include <unistd.h>
#include <arken/base>

string os::executablePath()
{
  return os::target("/proc/curproc/exe");
}

size_t os::mem()
{
  struct rusage rusage;
  getrusage( RUSAGE_SELF, &rusage );
  return (size_t)(rusage.ru_maxrss);
}

const char * os::name()
{
  static const char * s_name = "netbsd";
  return s_name;
}

const char * os::cext()
{
  static const char * s_cext= "so";
  return s_cext;
}

uint64_t os::pid()
{
  return getpid();
}
