#include <stdlib.h>
#include <string.h>

char *
oberon_string_simplified(const char *string)
{

  int i = 0;
  int j = 0;
  int f = 0;
  int len = strlen(string);
  char swap;
  char * res = (char *) malloc(len * sizeof(char));

  while(i < len) {
    if (string[i] <= ' ') {
      if(f) {
        res[j] = ' ';
        f = 0;
        ++j;
      }
      ++i;
    } else {
      swap = string[i];
      res[j] = swap;
      ++i;
      ++j;
      f = 1;
    }
  }

  if(string[j-1] <= ' ') {
    res[j-1] = '\0';
  } else {
    res[j] = '\0';
  }

  return res;
}
