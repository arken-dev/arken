#include <arken/base>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/resource.h>

string os::hostname()
{
  auto hostname = new char[HOST_NAME_MAX+1]();
  gethostname(hostname, HOST_NAME_MAX+1);
  return string(std::move(hostname));
}

string os::executablePath()
{
  return os::target("/proc/self/exe");
}

// https://gist.github.com/gbmhunter/00c57b55e2616cd8e1f21f77b79e59fc

size_t parseLine(char *line) {
  // This assumes that a digit will be found and the line ends in " Kb".
  int i = strlen(line);
  const char *p = line;
  while (*p < '0' || *p > '9') p++;
    line[i - 3] = '\0';
  i = atoi(p);
  return i;
}

size_t os::mem()
{
  //struct rusage rusage;
  //getrusage( RUSAGE_THREAD, &rusage );
  //return (size_t)(rusage.ru_maxrss);
  size_t result{0};
  FILE *file = fopen("/proc/self/status", "r");
  char line[128];
  while (fgets(line, 128, file) != nullptr) {
    if (strncmp(line, "VmRSS:", 6) == 0) {
      result = static_cast<size_t>(parseLine(line));
      break;
    }
  }
  fclose(file);
  return result;
}

const char * os::name()
{
  static const char * s_name = "linux";
  return s_name;
}

const char * os::cext()
{
  static const char * s_cext = "so";
  return s_cext;
}

uint64_t os::pid()
{
  return getpid();
}
