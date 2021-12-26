#include <arken/base>

string os::executablePath()
{
  std::cerr << "os.executablePath() not works in OpenBSD" << std::endl;
  return {};
}
