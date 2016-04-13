#include <stdlib.h>
#include <string.h>

char * oberon_string_truncate(const char *string, int pos)
{
   char * result;
   int i, len;

   len = strlen(string);
   if (pos > len) {
     pos = len;
   }
   result = malloc((pos+1) * sizeof(char));

   for (i = 0; i < pos; i++) {
     result[i] = string[i];
   }
   result[i] = '\0';

   return result;
}
