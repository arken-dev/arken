#include <oberon/base>
#include <oberon/algorithms/string/string-insert.hpp>
#include <oberon/algorithms/string/string-ends-with.hpp>

char * string::insert(const char * string, int len, const char * ba)
{
  return oberon_algorithms_string_insert(string, len, ba);
}

bool string::endsWith(const char * string, const char * str)
{
  return oberon_algorithms_string_ends_with(string, str);
}
