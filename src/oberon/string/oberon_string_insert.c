#include <string.h>
#include <stdlib.h>

char *
oberon_string_insert(const char * string, size_t len, const char * ba)
{
  size_t string_len = strlen(string);
  size_t ba_len     = strlen(ba);
  int size          = string_len + ba_len;
  char * result     = (char *) malloc((size + 1) * sizeof(char));
  size_t p, i, j;

  for(p=0, i=0; i < len; p++, i++){
    result[p] = string[i];
  }

  for(j=0; j < (ba_len) ; j++, p++){
    result[p] = ba[j];
  }

  while(i < string_len){
    result[p] = string[i];
    i++;
    p++;
  }

  result[p] = '\0';

  return result;
}
