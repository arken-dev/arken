#include <arken/base>
#include <libproc.h>
#include <unistd.h>
#include <sys/resource.h>

string os::executablePath()
{
  auto result = new char[PROC_PIDPATHINFO_MAXSIZE+1]();
  pid_t pid   = getpid();
  int ret     = proc_pidpath(pid, result, (sizeof(char) * PROC_PIDPATHINFO_MAXSIZE));
  if( ret <= 0 ) {
    delete[] result;
    return string();
  } else {
    return string(std::move(result));
  }
}

size_t os::mem()
{
  struct rusage rusage;
  getrusage( RUSAGE_SELF, &rusage );
  return (size_t)(rusage.ru_maxrss);
}

const char * os::name()
{
  static const char * name = "macos";
  return name;
}

const char * os::cext()
{
  static const char * s_cext = "dylib";
  return s_cext;
}

uint64_t os::pid()
{
  return getpid();
}
