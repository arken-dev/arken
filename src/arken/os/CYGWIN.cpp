#include <windows.h>
#include <winsock.h>
#include <psapi.h>
#include <process.h>
#include <arken/base>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/resource.h>


string os::hostname()
{
  #define ARKEN_HOSTNAME_MAX 64
  auto hostname = new char[ARKEN_HOSTNAME_MAX+1]();
  gethostname(hostname, ARKEN_HOSTNAME_MAX+1);
  return string(std::move(hostname));
}

string os::executablePath()
{
  auto path = new char[MAX_PATH+1]();
  GetModuleFileName(NULL, path, MAX_PATH);
  return string(std::move(path));
}

size_t os::mem()
{
  PROCESS_MEMORY_COUNTERS info;
  GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
  return (size_t)info.PeakWorkingSetSize;
}

const char * os::name()
{
  static const char * name = "windows";
  return name;
}

const char * os::cext()
{
  static const char * s_cext = "dll";
  return s_cext;
}

uint64_t os::pid()
{
  return getpid();
}
