#include <oberon/base>
#include <string.h>
#include <oberon/algorithms/string/string-insert.hpp>

char * string::insert(const char * string, int len, const char * ba)
{
  return oberon_algorithms_string_insert(string, len, ba);
}
