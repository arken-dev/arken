#include <windows.h>
#include <psapi.h>
#include <process.h>
#include <arken/base>

string os::executablePath()
{
  auto path = new char[_MAX_PATH+1]();
  GetModuleFileName(NULL, path, _MAX_PATH);
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
  static const char * s_ext = "dll";
  return s_cext;
}

uint64_t os::pid()
{
  return _getpid();
}
