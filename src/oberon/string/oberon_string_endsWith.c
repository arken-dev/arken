#include <string.h>

int oberon_string_endsWith(const char *string, const char *str)
{
   int string_len = strlen(string);
   int str_len    = strlen(str);
   int i;
   for (i=0; i <= str_len; i++) {
     if(string[string_len-i] != str[str_len-i]) {
       return 0;
     }
     if(i > str_len) {
       break;
     }
   }

   return 1;
}
