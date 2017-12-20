#include <charon/base>
#include <libproc.h>
#include <unistd.h>

char * os::executablePath()
{
  char * result = new char[PROC_PIDPATHINFO_MAXSIZE+1]();
  pid_t pid     = getpid();
  int ret       = proc_pidpath(pid, result, (sizeof(char) * PROC_PIDPATHINFO_MAXSIZE));
  if( ret <= 0 ) {
    delete[] result;
    return 0;
  } else {
    return result;
  }
}
