#include <oberon/base>
#include <oberon/string/oberon_string_append.hpp>
#include <oberon/string/oberon_string_insert.hpp>
#include <oberon/string/oberon_string_endsWith.hpp>

char * string::append(const char * string, const char * str)
{
  return oberon_string_append(string, str);
}

char * string::insert(const char * string, int len, const char * ba)
{
  return oberon_string_insert(string, len, ba);
}

bool string::endsWith(const char * string, const char * str)
{
  return oberon_string_endsWith(string, str);
}
