#include <arken/base>

string os::executablePath()
{
  return os::target("/proc/curproc/exe");
}
