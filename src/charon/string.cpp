// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <QDebug>

using ByteArrayList = charon::ByteArrayList;
using digest        = charon::digest;

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

static bool inline string_camelCase_special_char(char chr)
{
  if(chr == '_' || chr == ' ') {
    return true;
  } else {
    return false;
  }
}

char * string::camelCase(const char * string, bool lcfirst)
{
  int i, j;
  bool flag;
  int string_len;
  char * result;

  flag = true;
  string_len = strlen(string);
  result     = new char[string_len+1];

  for(i = 0, j = 0; i < string_len; i++) {
    if( string_camelCase_special_char(string[i]) ) {
      flag = true;
      i++;
    }

    if( string[i] == '/' ) {
      flag = true;
      result[j] = '.';
      i++;
      j++;
    }

    if( flag ) {
      result[j] = toupper(string[i]);
      flag = false;
    } else {
      result[j] = string[i];
    }

    j++;
  }

  if( lcfirst ) {
    result[0] = tolower(result[0]);
  }
  result[j] = '\0';

  return result;
}

char * string::capitalize(const char * string)
{
  int len = strlen(string);
  char * result  = new char[len + 1];

  strcpy(result, string);
  result[0] = toupper(string[0]);

  return result;
}

char * string::center(const char * string, size_t size, const char * pad)
{
  size_t str_len = strlen(string);
  size_t pad_len = strlen(pad);
  size_t mid_len = (size - str_len) / 2;
  char * result;

  if( size < str_len or pad_len == 0 ) {
    result = new char[ str_len + 1 ];
    strcpy(result, string);
    result[str_len] = '\0';
  } else {
    result = new char[ size + 1 ];

    size_t i = 0;
    size_t j = 0;

    for(i = 0; i < mid_len; i++, j++ ) {
      if( j >= pad_len ) {
        j = 0;
      }
      result[i] = pad[j];
    }

    for(j = 0; j < str_len; j++, i++ ) {
      result[i] = string[j];
    }

    for(j = 0; i < size ; i++, j++ ) {
      if( j >= pad_len ) {
        j = 0;
      }
      result[i] = pad[j];
    }
    result[size] = '\0';
  }

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

int string::count(const char * str1, const char * str2)
{
  size_t len1   = strlen(str1);
  size_t len2   = strlen(str2);
  int result    = 0;

  for( size_t i=0; i < len1; i++ ) {
    if (strncmp(&str1[i], str2, len2) == 0)
    {
      result++;
    }
  }

  return result;
}

char * string::chop(const char * string, int n)
{
  char * result;
  int len = strlen(string);

  if ( n > len ) {
    result = new char[1];
    result[0] = '\0';
  } else {
    int size = len - n;
    result = new char[size+1];
    int i;
    for( i = 0; i < size; i ++ ) {
      result[i] = string[i];
    }
    result[i] = '\0';
  }

  return result;
}

static inline int charon_string_dasherize_special_char(const char *string, int i)
{
  if(string[i] == '/' || string[i] == '_' || string[i] == '.' || string[i] == ' ') {
    return 1;
  } else {
    return 0;
  }
}

static inline int charon_string_dasherize_len(const char *string, int len)
{
  int i, j = 0, flag1 = 1, flag2 = 1;
  for(i = 0; i < len; i++) {
    if(isupper(string[i])) {
      if(flag1 == 0) {
        j++;
      }
      flag2 = 0;
      j++;
    } else {
      if(charon_string_dasherize_special_char(string, i)) {
        if(flag2 == 0) {
          j++;
          flag2 = 1;
          flag1 = 1;
        }
      } else {
        flag1 = 0;
        flag2 = 0;
        j++;
      }
    }
  }
  if(charon_string_dasherize_special_char(string, i-1)) {
    j--;
  }

  j++;
  return j;
}

char * string::dasherize(const char *string)
{
  int len = strlen(string);
  int i, j = 0, flag1 = 1, flag2 = 1;
  char * res = new char[charon_string_dasherize_len(string,len)+1];
  for(i = 0; i < len; i++) {
    if(isupper(string[i])) {
      if(flag1 == 0) {
        res[j] = '-';
        j++;
      }
      res[j] = tolower(string[i]);
      flag2 = 0;
      j++;
    } else {
      if(charon_string_dasherize_special_char(string, i)) {
        if(flag2 == 0) {
          res[j] = '-';
          j++;
          flag2 = 1;
          flag1 = 1;
        }
      } else {
        res[j] = string[i];
        flag1 = 0;
        flag2 = 0;
        j++;
      }
    }
  }
  if(charon_string_dasherize_special_char(string, i-1)) {
    j--;
  }
  res[j] = '\0';

  return res;
}
char * string::escape(const char * string)
{
  int i, j;
  int len   = strlen(string);
  int count = 0;
  char * result;

  for(i = 0; i < len; i++) {
    count++;
    if (string[i] == 34 || string[i] == 39 || string[i] == 92) {
      count++;
    }
  }

  result = new char[count+1];

  j = 0;
  for(i = 0; i < len; i++) {
    if (string[i] == 34 || string[i] == 39 || string[i] == 92) {
      result[j] = 92;
      j++;
    }
    result[j] = string[i];
    j++;
  }

  result[j] = '\0';

  return result;
}

char * string::escapeHtml(const char * string)
{
  int i, j;
  int len   = strlen(string);
  int count = 0;
  char * result;

  for(i = 0; i < len; i++) {
    switch(string[i]) {
      case 38:
        count += 5;
      break;
      case 34:
        count += 6;
      break;
      case 39:
        count += 6;
      break;
      case 60:
        count += 4;
      break;
      case 62:
        count += 4;
      break;
      default:
        count++;
    }
  }

  result = new char[count+1];

  j = 0;
  for(i = 0; i < len; i++) {
    switch(string[i]) {
      case 38:
        result[j] = '&';
        j++;
        result[j] = 'a';
        j++;
        result[j] = 'm';
        j++;
        result[j] = 'p';
        j++;
        result[j] = ';';
        j++;
      break;
      case 34:
        result[j] = '&';
        j++;
        result[j] = 'q';
        j++;
        result[j] = 'u';
        j++;
        result[j] = 'o';
        j++;
        result[j] = 't';
        j++;
        result[j] = ';';
        j++;
      break;
      case 39:
        result[j] = '&';
        j++;
        result[j] = '#';
        j++;
        result[j] = '0';
        j++;
        result[j] = '3';
        j++;
        result[j] = '9';
        j++;
        result[j] = ';';
        j++;
      break;
      case 60:
        result[j] = '&';
        j++;
        result[j] = 'l';
        j++;
        result[j] = 't';
        j++;
        result[j] = ';';
        j++;
      break;
      case 62:
        result[j] = '&';
        j++;
        result[j] = 'g';
        j++;
        result[j] = 't';
        j++;
        result[j] = ';';
        j++;
      break;
      default:
        result[j] = string[i];
        j++;
    }
  }

  result[j] = '\0';

  return result;
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

int string::indexOf(const char * string, const char * str, int i)
{
  int result;
  int j, c;
  int string_len = strlen(string);
  int str_len    = strlen(str);

  result = -1;

  for(; i < string_len; i++) {
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
  char * result;
  if ( len < 0 ) {
    len = string_len;
  }
  if ( pos >= string_len ) {
    len = 0;
  }
  result = new char[len + 1];
  for(i = 0; i < len; i++, j++) {
    result[j] = string[pos+i];
  }

  result[len] = '\0';

  return result;
}

static bool inline string_normalize_special_char(char chr)
{
  if(chr == '[' || chr == ']') {
    return true;
  } else {
    return false;
  }
}

char * string::normalize(const char *string)
{
  int i, j;
  bool flag;
  int string_len;
  char * result;

  flag = true;
  string_len = strlen(string);
  result     = new char[string_len+1];

  for(i = 0, j = 0; i < string_len; i++) {
    if( string_normalize_special_char(string[i]) ) {
      if(flag) {
        result[j] = '_';
        flag = false;
        j++;
      }
    } else {
      result[j] = string[i];
      j++;
      flag = true;
    }
  }


  if( result[j-1] == '_' ) {
    result[j-1] = '\0';
  } else {
    result[j] = '\0';
  }

  return result;
}

char * string::leftJustified(const char * string, size_t size, const char * pad)
{
  size_t str_len = strlen(string);
  size_t pad_len = strlen(pad);
  char * result;

  if( size < str_len or pad_len == 0 ) {
    result = new char[ str_len + 1 ];
    strcpy(result, string);
    result[str_len] = '\0';
  } else {
    result = new char[ size + 1 ];

    size_t i = 0;
    size_t j = 0;

    for(j = 0; j < str_len; j++, i++ ) {
      result[i] = string[j];
    }

    for(j = 0; i < size ; i++, j++ ) {
      if( j >= pad_len ) {
        j = 0;
      }
      result[i] = pad[j];
    }
    result[size] = '\0';
  }

  return result;
}

char * string::rightJustified(const char * string, size_t size, const char * pad)
{
  size_t str_len = strlen(string);
  size_t pad_len = strlen(pad);
  size_t mid_len = (size - str_len);
  char * result;

  if( size < str_len or pad_len == 0 ) {
    result = new char[ str_len + 1 ];
    strcpy(result, string);
    result[str_len] = '\0';
  } else {
    result = new char[ size + 1 ];

    size_t i = 0;
    size_t j = 0;

    for(i = 0; i < mid_len; i++, j++ ) {
      if( j >= pad_len ) {
        j = 0;
      }
      result[i] = pad[j];
    }

    for(j = 0; j < str_len; j++, i++ ) {
      result[i] = string[j];
    }

    result[size] = '\0';
  }

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

char * string::replace(const char *string, const char before, const char after, int start)
{
  int i, j;
  int string_len = strlen(string);
  char * result  = new char[string_len + 1];

  if ( start < 0 ) {
    start = string_len + start;
  }

  if ( start > 0 ) {
    for( int k = 0; k < start; k++) {
      result[k] = string[k];
    }
  }

  for(i = start, j = start; i < string_len; i++) {
    if( string[i] == before) {
      if( after != '\0' ) {
        result[j] = after;
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

static inline bool simplified_special_char(const char chr)
{
  if( chr >= 0 and chr <= 32 ) {
    return true;
  } else {
    return false;
  }
}

char * string::simplified(const char *string)
{
  int i = 0;
  int j = 0;
  int f = 0;
  int len;
  char * result;

  len = strlen(string);

  while(simplified_special_char(string[i])) {
    i++;
  }

  while(simplified_special_char(string[len-1])) {
    len--;
  }

  if( (len - i) <= 0 ) {
    result = new char[1];
    result[0] = '\0';
  } else {
    result = new char[len + 1];
    while(i < len) {
      if( simplified_special_char(string[i]) ) {
        if( f ) {
          i++;
        } else {
          f = 1;
          result[j] = ' ';
          i++;
          j++;
        }
      } else {
        result[j] = string[i];
        i++;
        j++;
        f = 0;
      }
    }

    result[j] = '\0';
  }

  return result;
}

//http://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
char * string::replace(const char * original, const char * pattern, const char * replacement, int start)
{
  size_t const replen = strlen(replacement);
  size_t const patlen = strlen(pattern);
  size_t const orilen = strlen(original);

  size_t patcnt = 0;
  const char * oriptr;
  const char * patloc;

  // find how many times the pattern occurs in the original string
  for (oriptr = original; (patloc = strstr(oriptr, pattern)); oriptr = patloc + patlen)
  {
    patcnt++;
  }

  {
    // allocate memory for the new string
    size_t const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

    if (returned != NULL)
    {
      if ( start < 0 ) {
        start = orilen + start;
      }

      if ( start > 0 ) {
        for( int k = 0; k < start; k++) {
          returned[k] = original[k];
        }
      }

      // copy the original string, 
      // replacing all the instances of the pattern
      char * retptr = returned + start;
      for (oriptr = original + start; (patloc = strstr(oriptr, pattern)); oriptr = patloc + patlen)
      {
        size_t const skplen = patloc - oriptr;
        // copy the section until the occurence of the pattern
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replacement 
        strncpy(retptr, replacement, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
    return returned;
  }
}

ByteArrayList * string::split(const char * raw, const char * pattern)
{
  return string::split(raw, strlen(raw), pattern);
}

ByteArrayList * string::split(const char * raw, size_t len, const char * pattern)
{
  ByteArrayList *list = new ByteArrayList();

  const char * current = raw;
  const char * other   = raw;
  int rawlen = len;
  int patternlen = strlen(pattern);
  int i, flag = 0, size = 0;

  for(i = 0; i < rawlen; i++) {
    if( strncmp(current, pattern, patternlen) == 0 ) {
      other = raw + flag;
      size = i - flag;
      if( size > 0 ) {
        //char * tmp = new char[size+1];
        //strncpy(tmp, other, size);
        //tmp[size] = '\0';
        list->append(other, size);
      }
      flag = i+patternlen;
    }
    current++;
  }

  if( flag < rawlen ) {
    other = raw + flag;
    size =  i - flag;
    //char * tmp = new char[size+1];
    //strncpy(tmp, other, size);
    //tmp[size] = '\0';
    list->append(other, size);
  }

  return list;
}

char * string::suffix(const char * raw)
{
  return string::suffix(raw, '.');
}

char *  string::suffix(const char * raw, char chr)
{
  char * result = 0;
  int i, j;
  int point = 0;
  int len = strlen(raw);

  for( i = len; i > 0; i--) {
    if( raw[i] == chr ) {
      point = i;
      break;
    }
  }

  if( point > 0 ) {
    point++;
    result = (char *) malloc( (len - point) + 1 * sizeof(char) );
    j = 0;
    for( i = point; i < len; i++, j++) {
      result[j] = raw[i];
    }
    result[(len - point)] = '\0';
  }

  return result;
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

static inline bool trim_special_char(const char chr)
{
  if( chr >= 0 and chr <= 32 ) {
    return true;
  } else {
    return false;
  }
}

char * string::trimmed(const char *string)
{
  int i = 0;
  int j = 0;
  int len;
  char * result;

  len = strlen(string);

  while(trim_special_char(string[i])) {
    i++;
  }

  while(trim_special_char(string[len])) {
    len--;
  }

  if( (len - i) < 0 ) {
    result = new char[1];
    result[0] = '\0';
  } else {
    result = new char[(len-i) + 2];
    while(i <= len) {
      result[j] = string[i];
      i++;
      j++;
    }

    result[j] = '\0';
  }

  return result;
}

char * string::leftTrimmed(const char *string)
{
  int i = 0;
  int j = 0;
  int len;
  char * result;

  len = strlen(string);

  while(trim_special_char(string[i])) {
    i++;
  }

  if( (len - i) < 0 ) {
    result = new char[1];
    result[0] = '\0';
  } else {
    result = new char[(len-i) + 2];
    while(i <= len) {
      result[j] = string[i];
      i++;
      j++;
    }

    result[j] = '\0';
  }

  return result;
}

char * string::rightTrimmed(const char *string)
{
  int i = 0;
  int j = 0;
  int len;
  char * result;

  len = strlen(string);

  while(trim_special_char(string[len])) {
    len--;
  }

  if( (len - i) < 0 ) {
    result = new char[1];
    result[0] = '\0';
  } else {
    result = new char[(len-i) + 2];
    while(i <= len) {
      result[j] = string[i];
      i++;
      j++;
    }

    result[j] = '\0';
  }

  return result;
}

char * string::truncate(const char *string, int pos, const char * omission, const char separator)
{
  char * result;
  int i, string_len, omission_len;

  string_len = strlen(string);

  if( omission == 0 ) {
    omission_len = 0;
  } else {
    omission_len = strlen(omission);
  }

  if ( pos > string_len ) {
    pos = string_len;
  }

  pos -= omission_len;

  i = 0;

  if ( separator != '\0' ) {
    for( i = pos; i > 0; i--) {
      if ( string[i] == separator ) {
        //i--;
        break;
      }
    }
  }

  if ( i > 0 ) {
    pos = i;
  }

  //pos += omission_len;

  result = new char[pos+omission_len+1];

  for (i = 0; i < pos; i++) {
    result[i] = string[i];
  }

  if ( omission_len > 0 ) {
    for( int j = 0; j < omission_len; j++, i++ ) {
      result[i] = omission[j];
    }
  }
  result[i] = '\0';

  return result;
}

static inline int charon_string_underscore_special_char(const char *string, int i)
{
  if(string[i] == '/' || string[i] == '_' || string[i] == '.' || string[i] == ' ') {
    return 1;
  } else {
    return 0;
  }
}

static inline int charon_string_underscore_len(const char *string, int len)
{
  int i, j = 0, flag1 = 1, flag2 = 1;
  for(i = 0; i < len; i++) {
    if(isupper(string[i])) {
      if(flag1 == 0) {
        j++;
      }
      flag2 = 0;
      j++;
    } else {
      if(charon_string_underscore_special_char(string, i)) {
        if(flag2 == 0) {
          j++;
          flag2 = 1;
          flag1 = 1;
        }
      } else {
        flag1 = 0;
        flag2 = 0;
        j++;
      }
    }
  }
  if(charon_string_underscore_special_char(string, i-1)) {
    j--;
  }

  j++;
  return j;
}

char * string::underscore(const char *string)
{
  int len = strlen(string);
  int i, j = 0, flag1 = 1, flag2 = 1;
  char * res = new char[charon_string_underscore_len(string,len)+1];
  for(i = 0; i < len; i++) {
    if(isupper(string[i])) {
      if(flag1 == 0) {
        res[j] = '_';
        j++;
      }
      res[j] = tolower(string[i]);
      flag2 = 0;
      j++;
    } else {
      if(charon_string_underscore_special_char(string, i)) {
        if(flag2 == 0) {
          res[j] = '_';
          j++;
          flag2 = 1;
          flag1 = 1;
        }
      } else {
        res[j] = string[i];
        flag1 = 0;
        flag2 = 0;
        j++;
      }
    }
  }
  if(charon_string_underscore_special_char(string, i-1)) {
    j--;
  }
  res[j] = '\0';

  return res;
}

char * string::md5(const char * str)
{
  return digest::md5(str);
}

char * string::sha1(const char * str)
{
  return digest::sha1(str);
}

//-----------------------------------------------------------------------------
// CLASS
//-----------------------------------------------------------------------------

string::string()
{
  m_reserve  = 1024;
  m_size     = 0;
  m_data     = new char[m_reserve]();
  m_capacity = m_size;
}

string::string(const char * data)
{
  m_reserve   = 1024;
  m_size      = strlen(data);
  m_capacity  = m_size;
  m_data      = new char[m_size+1];
  strcpy(m_data, data);
}

string::string(size_t reserve)
{
  m_reserve   = reserve;
  m_size      = 0;
  m_data      = new char[m_reserve]();
  m_capacity  = m_size;
}

string * string::consume(char * data)
{
  charon::string * tmp = new string;
  tmp->m_data = data;
  tmp->m_size = strlen(data);
  tmp->m_capacity = tmp->m_size;
  return tmp;
}

string::~string()
{
  delete[] m_data;
}

void string::append(const char * data)
{
  size_t len = strlen(data);

  if( (m_size + len) >= m_capacity ) {
    char * tmp = m_data;
    m_capacity = m_size + len + m_reserve;
    m_data     = new char[m_capacity];
    for(size_t i = 0; i < m_size; i++) {
      m_data[i] = tmp[i];
    }
    delete[] tmp;
  }

  for(size_t i=0; i < len; i++, m_size++) {
    m_data[m_size] = data[i];
  }

  m_data[m_size] = '\0';
}


string * string::camelCase(bool lcfirst)
{
  return charon::string::consume(string::camelCase(m_data, lcfirst));
}

string * string::capitalize()
{
  return charon::string::consume(string::capitalize(m_data));
}

string * string::center(size_t size, const char * pad)
{
  return charon::string::consume(string::center(m_data, size, pad));
}

bool string::contains(const char * str)
{
  return charon::string::contains(m_data, str);
}

string * string::chop(int n)
{
  return charon::string::consume(string::chop(m_data, n));
}

int string::count(const char * str)
{
  return string::count(m_data, str);
}

string * string::dasherize()
{
  return charon::string::consume(string::dasherize(m_data));
}

string * string::escape()
{
  return charon::string::consume(string::escape(m_data));
}

string * string::escapeHtml()
{
  return charon::string::consume(string::escapeHtml(m_data));
}

int string::indexOf(const char * str, int i)
{
  return string::indexOf(m_data, str, i);
}

string * string::insert(int len, const char * ba)
{
  return charon::string::consume(string::insert(m_data, len, ba));
}

bool string::endsWith(const char * ba)
{
  return string::endsWith(m_data, ba);
}

int string::lastIndexOf(const char * str)
{
  return string::lastIndexOf(m_data, str);
}

string * string::left(int len)
{
  return string::consume( string::left(m_data, len) );
}

string * string::leftJustified(size_t size, const char * pad)
{
  return string::consume( string::leftJustified(m_data, size, pad) );
}

string * string::mid(int pos, int len)
{
  return string::consume( string::mid(m_data, pos, len) );
}

string * string::md5()
{
  return charon::string::consume(string::md5(m_data));
}

string * string::normalize()
{
  return charon::string::consume(string::normalize(m_data));
}

string * string::simplified()
{
  return charon::string::consume(string::simplified(m_data));
}

string * string::repeated(int times)
{
  return charon::string::consume(string::repeated(m_data, times));
}

string * string::replace(const char * before, const char * after, int start)
{
  return charon::string::consume(string::replace(m_data, before, after, start));
}

string * string::replace(const char before, const char after, int start)
{
  return charon::string::consume(string::replace(m_data, before, after, start));
}

void string::reserve(size_t reserve)
{
  m_reserve = reserve;
}

size_t string::reserve()
{
  return m_reserve;
}

string * string::right(int len)
{
  return charon::string::consume(string::right(m_data, len));
}

string * string::rightJustified(size_t size, const char * pad)
{
  return charon::string::consume(string::rightJustified(m_data, size, pad));
}

string * string::sha1()
{
  return charon::string::consume(string::sha1(m_data));
}

string * string::suffix(const char chr)
{
  return charon::string::consume(string::suffix(m_data, chr));
}

char * string::data() const
{
  return this->m_data;
}

string * string::trimmed()
{
  return charon::string::consume(string::trimmed(m_data));
}

string * string::leftTrimmed()
{
  return charon::string::consume(string::leftTrimmed(m_data));
}

string * string::rightTrimmed()
{
  return charon::string::consume(string::rightTrimmed(m_data));
}

bool string::startsWith(const char * str)
{
  return string::startsWith(m_data, str);
}

string * string::truncate(int pos, const char *omission, const char separator)
{
  return charon::string::consume(string::truncate(m_data, pos, omission, separator));
}

string * string::underscore()
{
  return charon::string::consume(string::underscore(m_data));
}

ByteArrayList * string::split(const char * pattern)
{
  return string::split(m_data, m_size, pattern);
}

uint64_t string::size()
{
  return m_size;
}

std::ostream & operator<<(std::ostream & os, const charon::string * str)
{
   os << str->data();
   return os;
}
std::ostream & operator<<(std::ostream & os, const charon::string & str)
{
   os << str.data();
   return os;
}
