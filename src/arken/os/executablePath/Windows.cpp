#include <arken/base>
#include <windows.h>

string os::executablePath()
{
  auto path = new char[_MAX_PATH+1]();
  GetModuleFileName(NULL, path, _MAX_PATH);
  return string(std::move(path));
}
