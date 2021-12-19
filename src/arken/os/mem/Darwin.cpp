// https://qastack.com.br/programming/669438/how-to-get-memory-usage-at-runtime-using-c
#include <arken/base>
#include <sys/resource.h>

size_t os::mem()
{
  struct rusage rusage;
  getrusage( RUSAGE_SELF, &rusage );
  return (size_t)(rusage.ru_maxrss / 1024L);
}
