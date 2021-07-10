// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <iostream>
#include <cstring>

using arken::string;
using List = arken::string::List;

string *
checkString ( lua_State *L, int index = 1 ) {
  return *(string **) luaL_checkudata(L, index, "arken.string.metatable");
}

//-----------------------------------------------------------------------------
// ClassMethods
//-----------------------------------------------------------------------------

static int
arken_string_new( lua_State *L ) {
  string * str;
  if(lua_gettop(L) == 0) { // number of arguments
    str = new string();
  } else {
    size_t len;
    const char *s = luaL_checklstring(L, 1, &len);
    str = new string(s, len);
  }
  string **ptr = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = str;
  luaL_getmetatable(L, "arken.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_string_append( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  char *result = string::append(string, str);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_blank( lua_State *L ) {
  const char *str = luaL_checkstring(L, 1);
  bool result = string::blank(str);
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_string_camelCase( lua_State *L ) {
  char * result;
  const char *string = luaL_checkstring(L, 1);
  bool  flag = false;

  if(lua_gettop(L) == 2) { /* número de argumentos */
    flag =  lua_toboolean(L, 2);
  }

  result = string::camelCase(string, flag);

  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_capitalize( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char *result = string::capitalize(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_center( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int size           = luaL_checkinteger(L, 2);
  const char *pad    = luaL_checkstring(L, 3);
  char *result       = string::center(string, size, pad);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_chop( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int n =  luaL_checkinteger(L, 2);
  char * result = string::chop(string, n);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_md5( lua_State *L ) {
  size_t len;
  const char * str = luaL_checklstring(L, 1, &len);
  char * result = string::md5(str, len);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_sha1( lua_State *L ) {
  size_t len;
  const char * str = luaL_checklstring(L, 1, &len);
  char * result = string::sha1(str, len);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_contains( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  bool result        = string::contains(string, str);
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_string_decode64( lua_State *L ) {
  const char *str = luaL_checkstring(L, 1);
  size_t size;
  char * result   = string::decode64(str, &size);
  lua_pushlstring(L, result, size);
  delete[] result;
  return 1;
}

static int
arken_string_encode64( lua_State *L ) {
  size_t size;
  const char *str = luaL_checklstring(L, 1, &size);
  char * result   = string::encode64(str, size);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_decode( lua_State *L ) {
  const char *value   = luaL_checkstring(L, 1);
  const char *charset = luaL_checkstring(L, 2);
  char * result   = string::decode(value, charset);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_encode( lua_State *L ) {
  const char *value   = luaL_checkstring(L, 1);
  const char *charset = luaL_checkstring(L, 2);
  char * result   = string::encode(value, charset);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_equals( lua_State *L ) {
  if( lua_isuserdata(L, 2) ) {
    const char * str = luaL_checkstring(L, 1);
    string * udata = checkString( L, 2 );
    bool result = udata->equals(str);
    lua_pushboolean(L, result);
  } else {
    const char * str1 = luaL_checkstring(L, 1);
    const char * str2 = luaL_checkstring(L, 2);
    bool result = string::equals(str1, str2);
    lua_pushboolean(L, result);
  }

  return 1;
}

static int
arken_string_empty( lua_State *L ) {
  const char * str1 = luaL_checkstring(L, 1);
  bool result = string::empty(str1);
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_string_count( lua_State *L ) {
  const char *str1 = luaL_checkstring(L, 1);
  const char *str2 = luaL_checkstring(L, 2);
  int result       = string::count(str1, str2);
  lua_pushinteger(L, result);
  return 1;
}


static int
arken_string_endsWith( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  bool result        = string::endsWith(string, str);
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_string_escape( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char * result      = string::escape(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_escapeHtml( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char * result      = string::escapeHtml(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_hash( lua_State *L ) {
  size_t len;
  const char *str = luaL_checklstring(L, 1, &len);
  size_t result = string::hash(str, len);
  lua_pushnumber(L, result);
  return 1;
}

static int
arken_string_indexOf( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  int start = 0;
  if(lua_gettop(L) == 3) { /* número de argumentos */
    start = lua_tointeger(L, 3) - 1;
  }

  int result = string::indexOf(string, str, start);
  if( result > -1 ) {
    result ++;
  }
  lua_pushnumber(L, result);
  return 1;
}

static int
arken_string_insert( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int len            = luaL_checkinteger(L, 2);
  const char *ba     = luaL_checkstring(L, 3);
  char *result = string::insert(string, len-1, ba);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_lastIndexOf( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  int result = string::lastIndexOf(string, str);
  if( result > -1 ) {
    result ++;
  }
  lua_pushnumber(L, result);
  return 1;
}

static int
arken_string_left( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    len    =  luaL_checkinteger(L, 2);
  char * result = string::left(string, len);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_mid( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int pos      =  luaL_checkinteger(L, 2);
  int len;
  if(lua_gettop(L) == 3) { /* número de argumentos */
    len =  luaL_checkinteger(L, 3);
  } else {
    len = -1;
  }
  char *result = string::mid(string, pos-1, len);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_normalize( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::normalize(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_padLeft( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int size           = luaL_checkinteger(L, 2);
  const char *pad    = luaL_checkstring(L, 3);
  char *result       = string::padLeft(string, size, pad);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_prefix( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * pattern = luaL_checkstring(L, 2);;
  char * result = string::prefix(string, pattern);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}


static int
arken_string_padRight( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int size           = luaL_checkinteger(L, 2);
  const char *pad    = luaL_checkstring(L, 3);
  char *result       = string::padRight(string, size, pad);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_repeated( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    times  =  luaL_checkinteger(L, 2);
  char * result = string::repeated(string, times);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_right( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    len    =  luaL_checkinteger(L, 2);
  char * result = string::right(string, len);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_squish( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result = string::squish(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_startsWith( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * ba     = luaL_checkstring(L, 2);
  bool result         = string::startsWith(string, ba);
  lua_pushboolean(L, result);  /* push result */
  return 1;
}

static int
arken_string_suffix( lua_State *L ) {
  const char * string  = luaL_checkstring(L, 1);
  const char * pattern = luaL_checkstring(L, 2);
  char * result  = string::suffix(string, pattern);;
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_split( lua_State *L ) {
  size_t len;
  const char  * string  = luaL_checklstring(L, 1, &len);
  const char  * pattern = luaL_checkstring(L, 2);
  List list = string::split(string, len, pattern);
  List **ptr  = (List **)lua_newuserdata(L, sizeof(List*));
  *ptr = new List(list);
  luaL_getmetatable(L, "arken.string.List.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_string_replace( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * before = luaL_checkstring(L, 2);
  const char * after  = luaL_checkstring(L, 3);
  int start = 0;

  if(lua_gettop(L) > 3) { /* número de argumentos */
    start = luaL_checkinteger(L, 4);
    if( start >= 0 ) {
      start = start - 1;
    }
  }

  char * result = string::replace(string, before, after, start);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_replaceChar( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * before = luaL_checkstring(L, 2);
  const char * after  = luaL_checkstring(L, 3);
  int start = 0;

  if(lua_gettop(L) > 3) { /* número de argumentos */
    start = luaL_checkinteger(L, 4);
    if( start >= 0 ) {
      start = start - 1;
    }
  }

  char * result = string::replaceChar(string, before[0], after[0], start);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_trim( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::trim(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_trimLeft( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::trimLeft(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_trimRight( lua_State *L ) {
  const char * str = luaL_checkstring(L, 1);
  char * result    = string::trimRight(str);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
arken_string_truncate( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int pos = luaL_checkinteger(L, 2);
  const char * omission;
  char separator;

  if(lua_gettop(L) >= 3) { /* número de argumentos */
    omission = lua_tostring(L, 3);
  } else {
    omission = "...";
  }

  if(lua_gettop(L) == 4) { /* número de argumentos */
    separator = lua_tostring(L, 4)[0];
  } else {
    separator = ' ';
  }

  char  * result = string::truncate(string, pos, omission, separator);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_dasherize( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char       * result = string::dasherize(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_underscore( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char      * result = string::underscore(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static const luaL_reg
StringClassMethods[] = {
  {"new",            arken_string_new},
  {"append",         arken_string_append},
  {"blank",          arken_string_blank},
  {"camelCase",      arken_string_camelCase},
  {"capitalize",     arken_string_capitalize},
  {"center",         arken_string_center},
  {"contains",       arken_string_contains},
  {"chop",           arken_string_chop},
  {"count",          arken_string_count},
  {"endsWith",       arken_string_endsWith},
  {"escape",         arken_string_escape},
  {"escapeHtml",     arken_string_escapeHtml},
  {"hash",           arken_string_hash},
  {"encode",         arken_string_encode},
  {"decode",         arken_string_decode},
  {"encode64",       arken_string_encode64},
  {"equals",         arken_string_equals},
  {"empty",          arken_string_empty},
  {"decode64",       arken_string_decode64},
  {"dasherize",      arken_string_dasherize},
  {"indexOf",        arken_string_indexOf},
  {"insert",         arken_string_insert},
  {"left",           arken_string_left},
  {"lastIndexOf",    arken_string_lastIndexOf},
  {"md5",            arken_string_md5},
  {"mid",            arken_string_mid},
  {"normalize",      arken_string_normalize},
  {"padLeft",        arken_string_padLeft},
  {"padRight",       arken_string_padRight},
  {"prefix",         arken_string_prefix},
  {"squish",         arken_string_squish},
  {"repeated",       arken_string_repeated},
  {"replace",        arken_string_replace},
  {"replaceChar",    arken_string_replaceChar},
  {"right",          arken_string_right},
  {"sha1",           arken_string_sha1},
  {"startsWith",     arken_string_startsWith},
  {"split",          arken_string_split},
  {"suffix",         arken_string_suffix},
  {"trim",           arken_string_trim},
  {"trimLeft",       arken_string_trimLeft},
  {"trimRight",      arken_string_trimRight},
  {"truncate",       arken_string_truncate},
  {"underscore",     arken_string_underscore},
  // deprecate methods
  {"trimmed",        arken_string_trim},
  {"leftTrimmed",    arken_string_trimLeft},
  {"rightTrimmed",   arken_string_trimRight},
  {"simplified",     arken_string_squish},
  {"leftJustified",  arken_string_padLeft},
  {"rightJustified", arken_string_padRight},
  {NULL, NULL}
};

void static
registerStringClassMethods( lua_State *L ) {
  luaL_register(L, "string", StringClassMethods);
}

//-----------------------------------------------------------------------------
// InstanceMethods
//-----------------------------------------------------------------------------

static int
arken_StringInstanceMethodCenter( lua_State *L ) {
  string * udata  = checkString( L );
  int size        = luaL_checkinteger(L, 2);
  const char *pad = luaL_checkstring(L, 3);
  string result = udata->center(size, pad);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodClear( lua_State *L ) {
  string * udata = checkString( L );
  udata->clear();
  return 0;
}

static int
arken_StringInstanceMethodBlank( lua_State *L ) {
  string * udata = checkString( L );
  bool result = udata->blank();
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_StringInstanceMethodCamelCase( lua_State *L ) {
  string * udata = checkString( L );
  bool     flag  = false;

  if(lua_gettop(L) == 2) { /* número de argumentos */
    flag =  lua_toboolean(L, 2);
  }

  string result = udata->camelCase(flag);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodCapitalize( lua_State *L ) {
  string * udata = checkString( L );

  string result = udata->capitalize();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}


static int
arken_StringInstanceMethodContains( lua_State *L ) {
  string * udata  = checkString( L );
  const char *pad = luaL_checkstring(L, 2);
  lua_pushboolean(L, udata->contains(pad));
  return 1;
}

static int
arken_StringInstanceMethodCount( lua_State *L ) {
  string * udata  = checkString( L );
  const char *pad = luaL_checkstring(L, 2);
  lua_pushinteger(L, udata->count(pad));
  return 1;
}

static int
arken_StringInstanceMethodChop( lua_State *L ) {
  string * udata = checkString( L );
  int n = luaL_checkinteger(L, 2);
  string result = udata->chop(n);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodDasherize( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->dasherize();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodEscape( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->escape();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodEscapeHtml( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->escapeHtml();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodHash( lua_State *L ) {
  string * udata  = checkString( L );
  size_t   result = udata->hash();
  lua_pushnumber(L, result);
  return 1;
}

static int
arken_StringInstanceMethodIndexOf( lua_State *L ) {
  string * udata  = checkString( L );
  const char *pad = luaL_checkstring(L, 2);
  int start = 0;
  if(lua_gettop(L) == 3) { // number of arguments
    start = lua_tointeger(L, 3) - 1;
  }
  int result = udata->indexOf(pad, start);
  if( result > -1 ) {
    result ++;
  }
  lua_pushinteger(L, result);
  return 1;
}

static int
arken_StringInstanceMethodInsert( lua_State *L ) {
  string * udata = checkString( L );
  int len        = luaL_checkinteger(L, 2);
  const char *ba = luaL_checkstring(L, 3);
  string result  = udata->insert(len-1, ba);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodEndsWith( lua_State *L ) {
  string * udata  = checkString( L );
  const char * ba = luaL_checkstring(L, 2);
  bool result     = udata->endsWith(ba);
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_StringInstanceMethodLastIndexOf( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  int  result      = udata->lastIndexOf(str);
  lua_pushinteger(L, result);
  return 1;
}

static int
arken_StringInstanceMethodLeft( lua_State *L ) {
  string * udata = checkString( L );
  int len        = luaL_checkinteger(L, 2);
  string result  = udata->left(len);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodPadLeft( lua_State *L ) {
  string * udata   = checkString( L );
  size_t size      = luaL_checkinteger(L, 2);
  const char * pad = luaL_checkstring(L, 3);
  string result    = udata->padLeft(size, pad);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodMid( lua_State *L ) {
  string * udata = checkString( L );
  int pos = luaL_checkinteger(L, 2);
  int len = -1;
  if(lua_gettop(L) == 3) { // number of args
    len = luaL_checkinteger(L, 3);
  }
  string result    = udata->mid(pos-1, len);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodMd5( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->md5();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodNormalize( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->normalize();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodReplace( lua_State *L ) {
  string * udata = checkString( L );
  const char * before = luaL_checkstring(L, 2);
  const char * after  = luaL_checkstring(L, 3);
  int start = 0;

  if(lua_gettop(L) > 3) { /* número de argumentos */
    start = luaL_checkinteger(L, 4);
    if( start > 0 ) {
      start = start - 1;
    }
  }

  string result = udata->replace(before, after, start);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodReplaceChar( lua_State *L ) {
  string * udata = checkString( L );
  const char * before = luaL_checkstring(L, 2);
  const char * after  = luaL_checkstring(L, 3);
  int start = 0;

  if(lua_gettop(L) > 3) { /* número de argumentos */
    start = luaL_checkinteger(L, 4);
    if( start > 0 ) {
      start = start - 1;
    }
  }

  string result = udata->replace(before, after, start);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}


static int
arken_StringInstanceMethodRepeated( lua_State *L ) {
  string * udata = checkString( L );
  int times      = luaL_checkinteger(L, 2);
  string result  = udata->repeated(times);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodReserve( lua_State *L ) {
  string * udata = checkString( L );
  if(lua_gettop(L) > 1) { // number of arguments
   int reserve = luaL_checklong(L, 2);
    udata->reserve(reserve);
    return 0;
  } else {
    lua_pushnumber(L, udata->reserve());
    return 1;
  }
}

static int
arken_StringInstanceMethodRight( lua_State *L ) {
  string * udata = checkString( L );
  int len        = luaL_checkinteger(L, 2);
  string result  = udata->right(len) ;
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodPadRight( lua_State *L ) {
  string * udata   = checkString( L );
  size_t size      = luaL_checkinteger(L, 2);
  const char * pad = luaL_checkstring(L, 3);
  string result    = udata->padRight(size, pad);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodEncode64( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->encode64();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodDecode64( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->decode64();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodEncode( lua_State *L ) {
  string * udata = checkString( L );
  const char * charset = luaL_checkstring(L, 2);
  string result  = udata->encode(charset);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodPrefix( lua_State *L ) {
  string * udata = checkString( L );
  string result;
  const char * pattern = luaL_checkstring(L, 2);
  result = udata->prefix(pattern);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodDecode( lua_State *L ) {
  string * udata = checkString( L );
  const char * charset = luaL_checkstring(L, 2);
  string result  = udata->decode(charset);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodSha1( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->sha1();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodSquish( lua_State *L ) {
  string *  udata = checkString( L );
  string result = udata->squish();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodSuffix( lua_State *L ) {
  string * udata = checkString( L );
  const char * chr = luaL_checkstring(L, 2);
  string result = udata->suffix(chr);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodTrimLeft( lua_State *L ) {
  string *  udata = checkString( L );
  string result   = udata->trimLeft();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodTrim( lua_State *L ) {
  string *  udata = checkString( L );
  string result   = udata->trim();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodStartsWith( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  lua_pushboolean(L, udata->startsWith(str));
  return 1;
}

static int
arken_StringInstanceMethodTrimRight( lua_State *L ) {
  string *  udata = checkString( L );
  string result   = udata->trimRight();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodTruncate( lua_State *L ) {
  string *  udata = checkString( L );
  int pos = luaL_checkinteger(L, 2);
  const char * omission;
  char separator;

  if(lua_gettop(L) >= 3) { // number of arguments
    omission = lua_tostring(L, 3);
  } else {
    omission = "...";
  }

  if(lua_gettop(L) == 4) { /* número de argumentos */
    separator = lua_tostring(L, 4)[0];
  } else {
    separator = ' ';
  }

  string result = udata->truncate(pos, omission, separator);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodUnderscore( lua_State *L ) {
  string *  udata = checkString( L );
  string result   = udata->underscore();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_StringInstanceMethodAppend( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  udata->append(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
arken_StringInstanceMethodPrepend( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  udata->prepend(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
arken_StringInstanceMethodSplit( lua_State *L ) {
  string * udata = checkString( L );
  const char  * pattern = luaL_checkstring(L, 2);
  List list  = udata->split(pattern);
  List **ptr = (List **)lua_newuserdata(L, sizeof(List*));
  *ptr = new List(list);
  luaL_getmetatable(L, "arken.string.List.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_StringInstanceMethodToString( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushlstring(L, udata->data(), udata->size());
  return 1;
}

static int
arken_StringInstanceMethodEquals( lua_State *L ) {
  string * udata = checkString( L );
  if( lua_isuserdata(L, 2) ) {
    string * odata = checkString( L, 2 );
    lua_pushboolean(L, udata->equals(*odata));
  } else {
    const char * pattern = luaL_checkstring(L, 2);
    lua_pushboolean(L, udata->equals(pattern));
  }
  return 1;
}

static int
arken_StringInstanceMethodToLen( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushinteger(L, udata->len());
  return 1;
}

static int
arken_StringInstanceMethodSize( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static int
arken_StringInstanceMethodLen( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushinteger(L, udata->len());
  return 1;
}

static int
arken_StringInstanceMethodDestruct( lua_State *L ) {
  string * udata = checkString( L );
  delete udata;
  return 0;
}

static const
luaL_reg StringInstanceMethods[] = {
  {"append",         arken_StringInstanceMethodAppend},
  {"blank",          arken_StringInstanceMethodBlank},
  {"center",         arken_StringInstanceMethodCenter},
  {"chop",           arken_StringInstanceMethodChop},
  {"clear",          arken_StringInstanceMethodClear},
  {"camelCase",      arken_StringInstanceMethodCamelCase},
  {"capitalize",     arken_StringInstanceMethodCapitalize},
  {"contains",       arken_StringInstanceMethodContains},
  {"count",          arken_StringInstanceMethodCount},
  {"dasherize",      arken_StringInstanceMethodDasherize},
  {"escape",         arken_StringInstanceMethodEscape},
  {"decode",         arken_StringInstanceMethodDecode},
  {"encode",         arken_StringInstanceMethodEncode},
  {"decode64",       arken_StringInstanceMethodDecode64},
  {"encode64",       arken_StringInstanceMethodEncode64},
  {"escapeHtml",     arken_StringInstanceMethodEscapeHtml},
  {"hash",           arken_StringInstanceMethodHash},
  {"indexOf",        arken_StringInstanceMethodIndexOf},
  {"endsWith",       arken_StringInstanceMethodEndsWith},
  {"equals",         arken_StringInstanceMethodEquals},
  {"lastIndexOf",    arken_StringInstanceMethodLastIndexOf},
  {"left",           arken_StringInstanceMethodLeft},
  {"mid",            arken_StringInstanceMethodMid},
  {"insert",         arken_StringInstanceMethodInsert},
  {"len",            arken_StringInstanceMethodLen},
  {"md5",            arken_StringInstanceMethodMd5},
  {"normalize",      arken_StringInstanceMethodNormalize},
  {"padLeft",        arken_StringInstanceMethodPadLeft},
  {"padRight",       arken_StringInstanceMethodPadRight},
  {"prepend",        arken_StringInstanceMethodPrepend},
  {"prefix",         arken_StringInstanceMethodPrefix},
  {"repeated",       arken_StringInstanceMethodRepeated},
  {"replace",        arken_StringInstanceMethodReplace},
  {"replaceChar",    arken_StringInstanceMethodReplaceChar},
  {"reserve",        arken_StringInstanceMethodReserve},
  {"right",          arken_StringInstanceMethodRight},


  {"sha1",           arken_StringInstanceMethodSha1},
  {"size",           arken_StringInstanceMethodSize},
  {"squish",         arken_StringInstanceMethodSquish},
  {"suffix",         arken_StringInstanceMethodSuffix},
  {"split",          arken_StringInstanceMethodSplit},
  {"trim",           arken_StringInstanceMethodTrim},
  {"trimLeft",       arken_StringInstanceMethodTrimLeft},
  {"trimRight",      arken_StringInstanceMethodTrimRight},
  {"truncate",       arken_StringInstanceMethodTruncate},
  {"underscore",     arken_StringInstanceMethodUnderscore},
  {"startsWith",     arken_StringInstanceMethodStartsWith},
  {"__tostring",     arken_StringInstanceMethodToString},
  {"__len",          arken_StringInstanceMethodToLen},
  {"__gc",           arken_StringInstanceMethodDestruct},

  //deprecated methods
  {"simplified",     arken_StringInstanceMethodSquish},
  {"trimmed",        arken_StringInstanceMethodTrim},
  {"leftTrimmed",    arken_StringInstanceMethodTrimLeft},
  {"rightTrimmed",   arken_StringInstanceMethodTrimRight},
  {"leftJustified",  arken_StringInstanceMethodPadLeft},
  {"rightJustified", arken_StringInstanceMethodPadRight},

  {NULL, NULL}
};

void static
registerStringInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.string.metatable");
  luaL_register(L, NULL, StringInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * checkList
 */

List *
checkList( lua_State *L ) {
  return *(List **) luaL_checkudata(L, 1, "arken.string.List.metatable");
}

/**
 * ClassMethods
 */

static int
arken_string_ListClassMethodNew( lua_State *L ) {
  List **ptr = (List **)lua_newuserdata(L, sizeof(List*));
  *ptr = new List();
  luaL_getmetatable(L, "arken.string.List.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_string_ListClassMethods[] = {
  {"new", arken_string_ListClassMethodNew},
  {NULL, NULL}
};

void static
registerStringListClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.string.List");
  luaL_register(L, NULL, arken_string_ListClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_string_ListInstanceMethodDestruct( lua_State *L ) {
  List *udata = checkList( L );
  delete udata;
  return 0;
}

static int
arken_string_ListInstanceMethodAt( lua_State *L ) {
  List * udata  = checkList( L );
  int pos = luaL_checkint(L, 2);
  int len;
  const char * str = udata->at(pos-1, &len);
  if( str == 0 ) {
    lua_pushnil(L);
  } else {
    lua_pushlstring(L, str, len);
  }
  return 1;
}

static int
arken_string_list_each( lua_State *L ) {
  int i = lua_upvalueindex(1);
  luaL_checktype(L, i, LUA_TLIGHTUSERDATA);
  List * ba  = (List *) lua_touserdata(L, i);
  const char * result = ba->each();
  if( result == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, result);
  }
  return 1;
}

static int
arken_string_list_ipairs( lua_State *L ) {
  int i = lua_upvalueindex(1);
  luaL_checktype(L, i, LUA_TLIGHTUSERDATA);
  List * ba  = (List *) lua_touserdata(L, i);
  const char * result = ba->each();
  if( result == NULL ) {
    lua_pushnil(L);
    return 1;
  } else {
    lua_pushinteger(L, ba->cursor());
    lua_pushstring(L, result);
    return 2;
  }
}


static int
arken_string_ListInstanceMethodEach( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushlightuserdata(L, udata);
  lua_pushcclosure(L, arken_string_list_each, 1);
  return 1;
}

static int
arken_string_ListInstanceMethodIpairs( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushlightuserdata(L, udata);
  lua_pushcclosure(L, arken_string_list_ipairs, 1);
  return 1;
}


static int
arken_string_ListInstanceMethodFirst( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushstring(L, udata->first());
  return 1;
}

static int
arken_string_ListInstanceMethodLast( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushstring(L, udata->last());
  return 1;
}

static int
arken_string_ListInstanceMethodReplace( lua_State *L ) {
  List * udata  = checkList( L );
  int pos = luaL_checkint(L, 2);
  const char * value = luaL_checkstring(L, 3);
  udata->replace(pos, value);
  return 0;
}

static int
arken_string_ListInstanceMethodJoin( lua_State *L ) {
  List * udata   = checkList( L );
  const char  * separator = luaL_checkstring(L, 2);
  char * result = udata->join(separator);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}


static int
arken_string_ListInstanceMethodAppend( lua_State *L ) {
  List * udata  = checkList( L );
  const char * value = luaL_checkstring(L, 2);
  udata->append(value);
  lua_pushvalue(L, -2);
  return 1;
}

static int
arken_string_ListInstanceMethodSize( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static const
luaL_reg arken_string_ListInstanceMethods[] = {
  {"append",  arken_string_ListInstanceMethodAppend},
  {"at",      arken_string_ListInstanceMethodAt},
  {"each",    arken_string_ListInstanceMethodEach},
  {"__ipairs", arken_string_ListInstanceMethodIpairs},
  {"first",   arken_string_ListInstanceMethodFirst},
  {"last",    arken_string_ListInstanceMethodLast},
  {"join",    arken_string_ListInstanceMethodJoin},
  {"size",    arken_string_ListInstanceMethodSize},
  {"replace", arken_string_ListInstanceMethodReplace},
  {"__gc",    arken_string_ListInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerStringListInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.string.List.metatable");
  luaL_register(L, NULL, arken_string_ListInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// REGISTERS
//-----------------------------------------------------------------------------

int luaopen_arken_string( lua_State *L ) {
  registerStringClassMethods(L);
  registerStringInstanceMethods(L);
  registerStringListClassMethods(L);
  registerStringListInstanceMethods(L);
  return 1;
}
