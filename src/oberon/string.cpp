#include <oberon/base>
#include <oberon/string/oberon_string_append.hpp>
#include <oberon/string/oberon_string_insert.hpp>
#include <oberon/string/oberon_string_endsWith.hpp>
#include <oberon/string/oberon_string_mid.hpp>

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

char * string::mid(const char * string, size_t string_len, int pos, int len = -1)
{
  return oberon_mid(string, string_len, pos, len);
}
