#include <arken/base>
#include <libproc.h>
#include <unistd.h>

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
