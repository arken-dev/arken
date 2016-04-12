#include <stdlib.h>

char * oberon_mid(char * string, size_t string_len, int pos, int len)
{
  int i, j = 0;
  char * result = malloc(len * sizeof(char));
  if ( len < 0 ) {
    len = string_len;
  }

  for(i = 0; i < len; i++, j++) {
    result[j] = string[pos+i];
  }

  result[j] = '\0';

  return result;
}
