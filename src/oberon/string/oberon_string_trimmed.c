#include <stdlib.h>
#include <string.h>

char * oberon_string_trimmed(const char *string)
{
   int i = 0;
   int j = 0;
   int len;
   char * result;

   len = strlen(string);

   while(string[i] <= ' ') {
     i++;
   }

   while(string[len - 1] <= ' ') {
     --len;
   }

   result = malloc(len - i * sizeof(char));

   while(i < len) {
     result[j] = string[i];
     i++;
     j++;
   }
   result[j] = '\0';

   return result;
}
