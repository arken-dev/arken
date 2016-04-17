#include <oberon/helper>

char * string::append(const char * string, const char * ba)
{
  int string_len = strlen(string);
  int ba_len     = strlen(ba);
  int result_len = string_len + ba_len;
  char * result  = new char[result_len+1];
  int i, j = 0;
  for(i = 0; i < string_len; i++){
    result[j] = string[i];
    j++;
  }
  for(i = 0; i < ba_len; i++){
    result[j] = ba[i];
    j++;
  }
  result[j] = '\0';

  return result;
}

bool string::contains(const char * string, const char * str)
{
  int length = strlen(string);
  int len    = strlen(str);
  int i, j;

  for(i = 0; i < length; i++) {
    for(j=0; j < len; j++) {
      if( str[j] != string[i+j] ) {
        break;
      }
    }
    if(j == len) {
      return 1;
    }
  }

  return 0;
}

bool string::endsWith(const char * string, const char * str)
{
   int string_len = strlen(string);
   int str_len    = strlen(str);
   int i;
   for (i=0; i <= str_len; i++) {
     if(string[string_len-i] != str[str_len-i]) {
       return false;
     }
     if(i > str_len) {
       break;
     }
   }

  return true;
}

int string::indexOf(const char * string, const char * str)
{
  int result;
  int i, j, c;
  int string_len = strlen(string);
  int str_len    = strlen(str);

  result = 0;

  for(i = 0; i < string_len; i++) {
    c = 0;
    for(j = 0 ; j < str_len; j++) {
      if( string[i+j] != str[j] ) {
        break;
      }
      c++;
    }
    if(c == str_len) {
      result = i;
      break;
    }
  }

  return result;
}

char * string::insert(const char * string, int len, const char * ba)
{
  int string_len = strlen(string);
  int ba_len     = strlen(ba);
  int size       = string_len + ba_len;
  char * result  = new char[size + 1];
  int p, i, j;

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

int string::lastIndexOf(const char * string, const char * str)
{
  int result;
  int i, j, c;
  int string_len = strlen(string);
  int str_len    = strlen(str);

  result = -1;

  for(i = (string_len - str_len); i >= 0; i--) {
    c = 0;
    for(j = 0 ; j < str_len; j++) {
      if( string[i+j] != str[j] ) {
        break;
      }
      c++;
    }
    if(c == str_len) {
      result = i;
      break;
    }
  }

  return result;
}

char * string::left(const char * string, int len)
{
  int i;
  int string_len = strlen(string);
  char * result  = new char[len+1];

  if( len > string_len ) {
    len = string_len;
  }

  for(i=0; i < len; i++) {
    result[i] = string[i];
  }
  result[i] = '\0';

  return result;
}

char * string::mid(const char * string, int pos, int len = -1)
{
  int i, j = 0;
  int string_len = strlen(string);
  char * result  = new char[len + 1];
  if ( len < 0 ) {
    len = string_len;
  }

  for(i = 0; i < len; i++, j++) {
    result[j] = string[pos+i];
  }

  result[j] = '\0';

  return result;
}

char * string::repeated(const char *string, int times)
{
  int len = strlen(string);
  int result_len = (len*times);
  char * result = new char[result_len + 1];
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

char * string::replace(const char *string, const char before, const char * after)
{
  int i, j;
  int string_len = strlen(string);
  char * result  = new char[string_len + 1];

  for(i = 0, j = 0; i < string_len; i++) {
    if( string[i] == before) {
      if( ! after[0] == '\0' ) {
        result[j] = after[0];
        j++;
      }
    } else {
      result[j] = string[i];
      j++;
    }
  }
  result[j] = '\0';

  return result;
}

char * string::right(const char *string, int len)
{
  int i, j, string_len;
  char * result;

  string_len = strlen(string);
  j = string_len - len;

  result = new char[len + 1];

  for(i = 0; j < string_len; i++, j++){
    result[i] = string[j];
  }

  result[i] = '\0';

  return result;
}

char * string::simplified(const char *string)
{
  int i = 0;
  int j = 0;
  int f = 0;
  int len = strlen(string);
  char swap;
  char * res = new char[len + 1];

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

  if(string[j] <= ' ') {
    res[j-1] = '\0';
  } else {
    res[j] = '\0';
  }

  return res;
}

bool string::startsWith(const char *string, const char *str)
{
  int i;
  int str_len = strlen(str);
  int len = str_len - 1;

  for (i=0; i <= len; i++) {
    if(string[i] != str[i]) {
      return false;
    }
    if(i > str_len) {
      break;
    }
  }

  return true;
}

char * string::trimmed(const char *string)
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

  result = new char[len - i];

  while(i < len) {
    result[j] = string[i];
    i++;
    j++;
  }
  result[j] = '\0';

  return result;
}

char * string::truncate(const char *string, int pos)
{
  char * result;
  int i, len;

  len = strlen(string);
  if (pos > len) {
    pos = len;
  }
  result = new char[pos+1];

  for (i = 0; i < pos; i++) {
    result[i] = string[i];
  }
  result[i] = '\0';

  return result;
}

static inline int oberon_string_underscore_special_char(const char *string, int i)
{
  if(string[i] == '/' || string[i] == '_' || string[i] == '.' || string[i] == ' ') {
    return 1;
  } else {
    return 0;
  }
}

static inline int oberon_string_underscore_len(const char *string, int len)
{
  int i, j = 0, flag = 1;
  for(i = 0; i < len; i++) {
    if(isupper(string[i])) {
      if(flag == 0) {
        j++;
      }
      j++;
    } else {
      if(oberon_string_underscore_special_char(string, i)) {
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

char * string::underscore(const char *string)
{
  int len = strlen(string);
  int i, j = 0, flag = 1;
  char * res = new char[oberon_string_underscore_len(string,len)];
  for(i = 0; i < len; i++) {
    if(isupper(string[i])) {
      if(flag == 0) {
        res[j] = '_';
        j++;
      }
      res[j] = tolower(string[i]);
      flag = 0;
      j++;
    } else {
      if(oberon_string_underscore_special_char(string, i)) {
        if(flag == 0) {
          res[j] = '_';
          j++;
          flag = 1;
        }
      } else {
        res[j] = string[i];
        flag = 0;
        j++;
      }
    }
  }
  res[j] = '\0';

  return res;
}
