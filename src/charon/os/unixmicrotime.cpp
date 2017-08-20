#include <charon/base>

extern "C" {
#include <time.h>
#include <sys/time.h>
}

double os::microtime()
{
  struct timeval v;
  gettimeofday(&v, (struct timezone *) NULL);
  /* Unix Epoch time (time since January 1, 1970 (UTC)) */
  return v.tv_sec + v.tv_usec/1.0e6;
}
