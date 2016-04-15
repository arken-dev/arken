#include <string.h>

int oberon_string_contains(const char * string, const char * str)
{
  int length = strlen(string);
  int len    = strlen(str);
  int i, j;

  for(i = 0; i < length; i++) {
    for(j=0; j < len; j++) {
      if( str[j] != string[i+j] ) {
        break;
      }
    }
    if(j == len) {
      return 1;
    }
  }

  return 0;
}
