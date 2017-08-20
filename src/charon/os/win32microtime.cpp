#include <charon/base>

extern "C" {
#include <windows.h>
}

double os::microtime()
{
  FILETIME ft;
  double t;
  GetSystemTimeAsFileTime(&ft);
  /* Windows file time (time since January 1, 1601 (UTC)) */
  t  = ft.dwLowDateTime/1.0e7 + ft.dwHighDateTime*(4294967296.0/1.0e7);
  /* convert to Unix Epoch time (time since January 1, 1970 (UTC)) */
  return (t - 11644473600.0);
}
