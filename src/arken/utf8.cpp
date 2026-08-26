// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// https://stackoverflow.com/questions/36897781/how-to-uppercase-lowercase-utf-8-characters-in-c
// https://www.py4u.net/discuss/75078
// https://www.ime.usp.br/~pf/algoritmos/apend/unicode.html
// https://pt.wikipedia.org/wiki/ISO/IEC_8859-1
// https://www.asciitable.com/
// https://cs.stanford.edu/people/miles/iso8859.html
// https://www.utf8-chartable.de/
// https://github.com/dart-lang/http/issues/175

#include <cstdint>

#include <arken/base>
#include <arken/utf8>

namespace arken {

char * utf8::sub(const char * str, int p1, int p2)
{

  int i  = 0;
  int j  = 0;
  int v1 = 0;
  int v2 = 0;
  int len = utf8::len(str);

  if( p2 < 0 ) {
    p2 = len - 1;
  }

  if( p2 > len ) {
    p2 = len -1;
  }

  while( str[i] ) {

    if( j == p1 ) {
      v1 = i;
    }

    if( str[i] == -61 ) {
      i++;
    }

    if( j == p2 ) {
      v2 = i;
    }

    i++;
    j++;

  }

  int size   = (v2 - v1) + 1;

  if( size < 0 ) {
    size = 0;
  }

  auto res = new char[size+1];
  int a = 0;
  for(int b=v1; a < size; a++, b++) {
    res[a] = str[b];
  }

  res[size] = '\0';

  return res;
}

char * utf8::mid(const char * str, int p1, int p2)
{

  if( p2 < 0 ) {
    p2 = (utf8::len(str) - 1);
  } else {
    p2 = p1 + p2;
  }

  return utf8::sub(str, p1, p2);
}


char * utf8::upper(const char * str)
{
  size_t i   = 0;
  size_t len = strlen(str);
  auto res = new char[len+1];

  while( str[i] ) {

    if( str[i] == -61 && (str[i+1] >= -95 && str[i+1] <= -67) ) {
      res[i] = str[i];
      i++;
      res[i] = str[i] - 32;
    } else if( str[i] >= 97 && str[i] <= 122 ) {
      res[i] = str[i] - 32;
    } else {
      res[i] = str[i];
    }

    i++;
  }

  res[i] = '\0';

  return res;
}

char * utf8::lower(const char * str)
{
  size_t i   = 0;
  size_t len = strlen(str);
  auto res = new char[len+1];

  while( str[i] ) {

    if( str[i] == -61 && (str[i+1] >= -127 && str[i+1] <= -99) ) {
      res[i] = str[i];
      i++;
      res[i] = str[i] + 32;
    } else if( str[i] >= 65 && str[i] <= 90 ) {
      res[i] = str[i] + 32;
    } else {
      res[i] = str[i];
    }

    i++;
  }

  res[i] = '\0';

  return res;
}

int utf8::len(const char * string)
{
   int i = 0, j = 0;
   while (string[i]) {
     if ((string[i] & 0xc0) != 0x80) j++;
     i++;
   }
  return j;
}

char * utf8::decode(const char * str)
{
  int c = 0;
  int i = 0;
  int j = 0;

  while(str[i]) {
    if( str[i] == -61 ) {
      c++;
    }
    i++;
  }

  auto res = new char[(i-c)+1];

  i = 0;

  while( str[i] ) {
    if( str[i] == -61 ) {
      i++;
      res[j] = str[i] + 64;
    } else {
      res[j] = str[i];
    }
    i++;
    j++;
  }
  res[j] = '\0';

  return res;
}

char * utf8::encode(const char * str)
{
  int c = 0;
  int i = 0;
  int j = 0;

  while(str[i]) {
    if( str[i] < 0 ) {
      c++;
    }
    i++;
  }

  auto res = new char[(i+c)+1];

  i = 0;

  while( str[i] ) {
    if( str[i] < 0 ) {
      res[j] = -61;
      j++;
      res[j] = str[i] - 64;
    } else {
      res[j] = str[i];
    }
    i++;
    j++;
  }
  res[j] = '\0';

  return res;
}

char * utf8::sanitize(const char * str)
{
  size_t len = strlen(str);
  auto res = new char[len+1];
  int i = 0, j = 0;
  while (str[i]) {
    if ( str[i] == -61 ) {
      if ( str[i + 1] < 0 && str[i + 1] != -61 ) {
        res[j] = str[i];
        j++; i++;
        res[j] = str[i];
        j++; i++;
      } else {
        i++;
      }
    } else {
      if( str[i] > 0 ) {
        res[j] = str[i];
        j++; i++;
      } else {
        i++;
      }
    }
  }
  res[j] = '\0';
  return res;
}

char * utf8::asc(const char * str, char chr)
{
  int c = 0;
  int i = 0;
  int j = 0;

  while(str[i]) {
    if( str[i] == -61 ) {
      c++;
    }
    i++;
  }

  auto res = new char[(i-c)+1];

  i = 0;

  while( str[i] ) {
    if( str[i] == -61 ) {
      i++;
      if( str[i] >= -128 && str[i] <= -123 ) {
        //ÀÁÂÃÄÅ
        res[j] = 'A';
      } else if ( str[i] == -121 ) {
        //Ç
        res[j] = 'C';
      } else if ( str[i] >= -120 && str[i] <= -117 ) {
        //ÈÉÊË
        res[j] = 'E';
      } else if ( str[i] >= -116 && str[i] <= -113 ) {
        //ÌÍÎÏ
        res[j] = 'I';
      } else if ( str[i] == -112 ) {
        //Ð
        res[j] = 'D';
      } else if ( str[i] == -111 ) {
        //Ñ
        res[j] = 'N';
      } else if ( str[i] >= -110 && str[i] <= -106 ) {
        //ÒÓÔÕÖ
        res[j] = 'O';
      } else if ( str[i] == -105 ) {
        //×
        res[j] = 'x';
      } else if ( str[i] == -104 ) {
        //Ø
        res[j] = 'O';
      } else if ( str[i] >= -103 && str[i] <= -100 ) {
        //ÙÚÛÜ
        res[j] = 'U';
      } else if ( str[i] == -99 ) {
        //Ý
        res[j] = 'Y';
      } else if ( str[i] >= -96 && str[i] <= -91 ) {
        //àáâãäå
        res[j] = 'a';
      } else if ( str[i] == -89 ) {
        //ç
        res[j] = 'c';
      } else if ( str[i] >= -88 && str[i] <= -85 ) {
        //èéêë
        res[j] = 'e';
      } else if ( str[i] >= -84 && str[i] <= -81 ) {
        //ìíîï
        res[j] = 'i';
      } else if ( str[i] == -79 ) {
        //ñ
        res[j] = 'n';
      } else if ( str[i] >= -78 && str[i] <= -74 ) {
        //òóôõö
        res[j] = 'o';
      } else if ( str[i] == -72 ) {
        //ø
        res[j] = 'o';
      } else if ( str[i] >= -71 && str[i] <= -68 ) {
        //ùúûü
        res[j] = 'u';
      } else if ( str[i] == -67 || str[i] == -65 ) {
        //ýÿ
        res[j] = 'y';
      } else {
        if ( chr > 0 ) {
          res[j] = chr;
        } else {
          j--;
        }
      }
    } else {
      res[j] = str[i];
    }
    j++;
    i++;
  }
  res[j] = '\0';

  return res;
}

// RFC 3629: valida se um buffer é UTF-8 bem formado. Recebe len em vez de
// depender de terminador nulo porque UTF-8 válido pode conter bytes \0.
bool utf8::valid(const char * data, size_t len)
{
  size_t i = 0;

  while( i < len ) {
    auto byte = static_cast<uint8_t>(data[i]);

    size_t   extra;
    uint32_t codepoint;
    uint32_t minValue;

    if( (byte & 0x80) == 0x00 ) {
      i++;
      continue;
    } else if( (byte & 0xE0) == 0xC0 ) {
      extra = 1; codepoint = byte & 0x1F; minValue = 0x80;
    } else if( (byte & 0xF0) == 0xE0 ) {
      extra = 2; codepoint = byte & 0x0F; minValue = 0x800;
    } else if( (byte & 0xF8) == 0xF0 ) {
      extra = 3; codepoint = byte & 0x07; minValue = 0x10000;
    } else {
      return false; // byte de continuação sozinho, ou 0xF8-0xFF (nunca válido em UTF-8)
    }

    if( i + extra >= len ) {
      return false; // sequência cortada, faltam bytes de continuação
    }

    for(size_t j = 1; j <= extra; j++) {
      auto cont = static_cast<uint8_t>(data[i + j]);
      if( (cont & 0xC0) != 0x80 ) {
        return false; // esperava byte de continuação (10xxxxxx), não veio
      }
      codepoint = (codepoint << 6) | (cont & 0x3F);
    }

    if( codepoint < minValue ) {
      return false; // encoding "overlong" (mesmo code point codificado com mais bytes do que precisa)
    }

    if( codepoint >= 0xD800 && codepoint <= 0xDFFF ) {
      return false; // metade de surrogate pair - inválido em UTF-8 (só existe em UTF-16)
    }

    if( codepoint > 0x10FFFF ) {
      return false; // além do range válido do Unicode
    }

    i += extra + 1;
  }

  return true;
}

} // namespace arken
