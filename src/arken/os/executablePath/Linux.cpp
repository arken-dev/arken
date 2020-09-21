#include <charon/base>

char * os::executablePath()
{
  return os::target("/proc/self/exe");
}
