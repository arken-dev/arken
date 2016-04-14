#include <stdlib.h>
#include <string.h>

char * oberon_string_repeated(const char *string, int times)
{
   int len = strlen(string);
   size_t result_len = (len*times);
   char * result     = malloc( (result_len + 1) * sizeof(char));
   int i = 0, j = 0;
   while( i < result_len) {
     for(j=0; j < len; j++) {
       result[i] = string[j];
       i++;
     }
   }
   result[i] = '\0';

   return result;
}
