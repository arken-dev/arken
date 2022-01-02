#include <arken/base>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/syslimits.h>

string os::hostname()
{
  #define HOST_NAME_MAX 255
  auto hostname = new char[HOST_NAME_MAX+1]();
  gethostname(hostname, HOST_NAME_MAX+1);
  return string(std::move(hostname));
}

string os::executablePath()
{
  char temp[PATH_MAX];
  auto exepath = new char[PATH_MAX]();
  ::snprintf(temp, sizeof(temp),"/proc/%d/file", ::getpid());
  if( os::exists(temp) ) {
    ::realpath(temp, exepath);
    return string(std::move(exepath));
  } else {
    std::cerr << "mount -t procfs proc /proc "
              << "(is required for os.executaPath())"
              << std::endl;
    return {};
  }
}

size_t os::mem()
{
  struct rusage rusage;
  getrusage( RUSAGE_SELF, &rusage );
  return (size_t)(rusage.ru_maxrss);
}

uint64_t os::pid()
{
  return getpid();
}

const char * os::name()
{
  static const char * s_name = "freebsd";
  return s_name;
}

const char * os::cext()
{
  static const char * s_cext = "so";
  return s_cext;
}
