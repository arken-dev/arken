#include <string.h>

char * oberon_string_replace(const char * string, const char before, const char * after)
{
  int i, j;
  int string_len = strlen(string);
  char * result  = (char *) malloc( string_len + 1 * sizeof(char) );

  for(i = 0, j = 0; i < string_len; i++) {
    if( string[i] == before) {
      if( ! after[0] == '\0' ) {
        result[j] = after[0];
        j++;
      }
    } else {
      result[j] = string[i];
      j++;
    }
  }
  result[j] = '\0';

  return result;
}
