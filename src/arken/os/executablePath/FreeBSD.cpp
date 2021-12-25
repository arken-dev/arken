#include <arken/base>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>

string os::executablePath()
{
  char temp[PATH_MAX];
  auto exepath = new char[PATH_MAX]();
  ::snprintf(temp, sizeof(temp),"/proc/%d/file", ::getpid());
  if( os::exists(temp) ) {
    ::realpath(temp, exepath);
    return string(std::move(exepath));
  } else {
    std::cerr << "mount -t procfs proc /proc "
              << "(is required for os.executaPath())"
              << std::endl;
    return {};
  }
}
