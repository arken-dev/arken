#include <stdlib.h>
#include <string.h>
#include <ctype.h>

inline int oberon_string_underscore_special_char(const char *buffer, int i)
{
  if(buffer[i] == '/' || buffer[i] == '_' || buffer[i] == '.' || buffer[i] == ' ') {
    return 1;
  } else {
    return 0;
  }
}

inline int oberon_string_underscore_len(const char *buffer, int len)
{
  int i, j = 0, flag = 1;
  for(i = 0; i < len; i++) {
    if(isupper(buffer[i])) {
      if(flag == 0) {
        j++;
      }
      j++;
    } else {
      if(oberon_string_underscore_special_char(buffer, i)) {
        if(flag == 0) {
          j++;
          flag = 1;
        }
      } else {
        flag = 0;
        j++;
      }
    }
  }
  j++;
  return j;
}

char *
oberon_string_underscore(const char *buffer)
{
  int len = strlen(buffer);
  int i, j = 0, flag = 1;
  char * res = (char *) malloc(oberon_string_underscore_len(buffer,len) * sizeof(char));
  for(i = 0; i < len; i++) {
    if(isupper(buffer[i])) {
      if(flag == 0) {
        res[j] = '_';
        j++;
      }
      res[j] = tolower(buffer[i]);
      flag = 0;
      j++;
    } else {
      if(oberon_string_underscore_special_char(buffer, i)) {
        if(flag == 0) {
          res[j] = '_';
          j++;
          flag = 1;
        }
      } else {
        res[j] = buffer[i];
        flag = 0;
        j++;
      }
    }
  }
  res[j] = '\0';

  return res;
}
