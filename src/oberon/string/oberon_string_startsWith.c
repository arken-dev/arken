#include <stdlib.h>
#include <string.h>

int oberon_string_startsWith(const char *string, const char *str)
{
   int i;
   int string_len = strlen(string);
   int str_len = strlen(str);
   int len = str_len - 1;

   /*
   if (str_len > string_len) {
     return 0;
   }
   */

   for (i=0; i <= len; i++) {
     if(string[i] != str[i]) {
       return 0;
     }
     if(i > str_len) {
       break;
     }
   }

   return 1;
}
