// https://qastack.com.br/programming/669438/how-to-get-memory-usage-at-runtime-using-c
#include <arken/base>
#include <windows.h>
#include <psapi.h>

size_t os::mem()
{
  PROCESS_MEMORY_COUNTERS info;
  GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
  return (size_t)info.PeakWorkingSetSize;
}
