#include <oberon/base>
#include <oberon/string/oberon_string_append.hpp>
#include <oberon/string/oberon_string_insert.hpp>
#include <oberon/string/oberon_string_endsWith.hpp>
#include <oberon/string/oberon_string_mid.hpp>
#include <oberon/string/oberon_string_repeated.hpp>
#include <oberon/string/oberon_string_right.hpp>
#include <oberon/string/oberon_string_simplified.hpp>
#include <oberon/string/oberon_string_startsWith.hpp>
#include <oberon/string/oberon_string_trimmed.hpp>
#include <oberon/string/oberon_string_truncate.hpp>

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
  return oberon_string_mid(string, string_len, pos, len);
}

char * string::repeated(char *string, int times)
{
  return oberon_string_repeated(string, times);
}

char * string::right(char *string, int len)
{
  return oberon_string_right(string, len);
}

char * string::simplified(char *string)
{
  return oberon_string_simplified(string);
}

bool string::startsWith(const char *string, const char *str)
{
  return oberon_string_startsWith(string, str);
}

char * string::trimmed(const char *string)
{
  return oberon_string_trimmed(string);
}

char * string::truncate(const char *string, int pos)
{
  return oberon_string_truncate(string, pos);
}
