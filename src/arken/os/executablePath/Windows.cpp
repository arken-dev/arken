#include <arken/base>
#include <windows.h>

char * os::executablePath()
{
  char * path = new char[_MAX_PATH+1];
  GetModuleFileName(NULL, path, _MAX_PATH);
  return path;
}
