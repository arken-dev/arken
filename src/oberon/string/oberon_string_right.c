#include <stdlib.h>
#include <string.h>

char * oberon_string_right(char *string, int len)
{
   int i, j, string_len;
   char * result;

   string_len = strlen(string);
   j = string_len - len;

   result = malloc((len + 1) * sizeof(char));

   for(i = 0; j < string_len; i++, j++){
     result[i] = string[j];
   }

   result[i] = '\0';

   return result;
}
