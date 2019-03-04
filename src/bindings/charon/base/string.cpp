// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <iostream>

using charon::string;
using List = charon::string::List;

string *
checkString ( lua_State *L ) {
  return *(string **) luaL_checkudata(L, 1, "charon.string.metatable");
}

//-----------------------------------------------------------------------------
// ClassMethods
//-----------------------------------------------------------------------------

static int
charon_string_new( lua_State *L ) {
  string * str;
  if(lua_gettop(L) == 0) { // number of arguments
    str = new string();
  } else {
    if( lua_isnumber(L, 1) ) {
      str = new string(lua_tointeger(L, 1));
    } else {
      const char *s = (char *) luaL_checkstring(L, 1);
      str = new string(s);
    }
  }
  string **ptr = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = str;
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_string_append( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  char *result = string::append(string, str);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_camelCase( lua_State *L ) {
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
charon_string_capitalize( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char *result = string::capitalize(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_center( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int size           = luaL_checkinteger(L, 2);
  const char *pad    = luaL_checkstring(L, 3);
  char *result       = string::center(string, size, pad);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_chop( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int n =  luaL_checkinteger(L, 2);
  char * result = string::chop(string, n);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_md5( lua_State *L ) {
  const char * str = luaL_checkstring(L, 1);
  char * result = string::md5(str);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_sha1( lua_State *L ) {
  const char * str = luaL_checkstring(L, 1);
  char * result = string::sha1(str);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_contains( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  bool result        = string::contains(string, str);
  lua_pushboolean(L, result);
  return 1;
}

static int
charon_string_decode64( lua_State *L ) {
  const char *str = luaL_checkstring(L, 1);
  char * result   = string::decode64(str);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_encode64( lua_State *L ) {
  const char *str = luaL_checkstring(L, 1);
  char * result   = string::encode64(str);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_count( lua_State *L ) {
  const char *str1 = luaL_checkstring(L, 1);
  const char *str2 = luaL_checkstring(L, 2);
  int result       = string::count(str1, str2);
  lua_pushinteger(L, result);
  return 1;
}


static int
charon_string_endsWith( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  const char *str    = luaL_checkstring(L, 2);
  bool result        = string::endsWith(string, str);
  lua_pushboolean(L, result);
  return 1;
}

static int
charon_string_escape( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char * result      = string::escape(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_escapeHtml( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char * result      = string::escapeHtml(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_indexOf( lua_State *L ) {
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
charon_string_insert( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int len            = luaL_checkinteger(L, 2);
  const char *ba     = luaL_checkstring(L, 3);
  char *result = string::insert(string, len-1, ba);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_lastIndexOf( lua_State *L ) {
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
charon_string_left( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    len    =  luaL_checkinteger(L, 2);
  char * result = string::left(string, len);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_mid( lua_State *L ) {
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
charon_string_normalize( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::normalize(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_leftJustified( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int size           = luaL_checkinteger(L, 2);
  const char *pad    = luaL_checkstring(L, 3);
  char *result       = string::leftJustified(string, size, pad);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_rightJustified( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int size           = luaL_checkinteger(L, 2);
  const char *pad    = luaL_checkstring(L, 3);
  char *result       = string::rightJustified(string, size, pad);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_repeated( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    times  =  luaL_checkinteger(L, 2);
  char * result = string::repeated(string, times);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_right( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    len    =  luaL_checkinteger(L, 2);
  char * result = string::right(string, len);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_simplified( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result = string::simplified(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_startsWith( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * ba     = luaL_checkstring(L, 2);
  bool result         = string::startsWith(string, ba);
  lua_pushboolean(L, result);  /* push result */
  return 1;
}

static int
charon_string_suffix( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char chr = '.';
  char * result;
  if(lua_gettop(L) == 2) { /* número de argumentos */
    chr =  luaL_checkstring(L, 2)[0];
  }
  result = string::suffix(string, chr);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_split( lua_State *L ) {
  size_t len;
  const char  * string  = luaL_checklstring(L, 1, &len);
  const char  * pattern = luaL_checkstring(L, 2);
  List * list = string::split(string, len, pattern);
  List **ptr  = (List **)lua_newuserdata(L, sizeof(List*));
  *ptr = list;
  luaL_getmetatable(L, "charon.string.List.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
charon_string_replace( lua_State *L ) {
  size_t len1, len2;
  const char * string = luaL_checkstring(L, 1);
  const char * before = luaL_checklstring(L, 2, &len1);
  const char * after  = luaL_checklstring(L, 3, &len2);
  int start = 0;

  if(lua_gettop(L) > 3) { /* número de argumentos */
    start = luaL_checkinteger(L, 4);
    if( start >= 0 ) {
      start = start - 1;
    }
  }

  char * result;
  if( len1 == 1 && len2 == 1) {
    result = string::replace(string, before[0], after[0], start);
  } else {
    result = string::replace(string, before, after, start);
  }
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_trimmed( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::trimmed(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_leftTrimmed( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::leftTrimmed(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_rightTrimmed( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::rightTrimmed(string);
  lua_pushstring(L, result);  /* push result */
  delete[] result;
  return 1;
}

static int
charon_string_truncate( lua_State *L ) {
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
charon_string_dasherize( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char       * result = string::dasherize(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_string_underscore( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  char      * result = string::underscore(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static const luaL_reg
StringClassMethods[] = {
  {"new",            charon_string_new},
  {"append",         charon_string_append},
  {"camelCase",      charon_string_camelCase},
  {"capitalize",     charon_string_capitalize},
  {"center",         charon_string_center},
  {"contains",       charon_string_contains},
  {"chop",           charon_string_chop},
  {"count",          charon_string_count},
  {"endsWith",       charon_string_endsWith},
  {"escape",         charon_string_escape},
  {"escapeHtml",     charon_string_escapeHtml},
  {"encode64",       charon_string_encode64},
  {"decode64",       charon_string_decode64},
  {"dasherize",      charon_string_dasherize},
  {"indexOf",        charon_string_indexOf},
  {"insert",         charon_string_insert},
  {"left",           charon_string_left},
  {"lastIndexOf",    charon_string_lastIndexOf},
  {"md5",            charon_string_md5},
  {"mid",            charon_string_mid},
  {"normalize",      charon_string_normalize},
  {"leftJustified",  charon_string_leftJustified},
  {"rightJustified", charon_string_rightJustified},
  {"simplified",     charon_string_simplified},
  {"repeated",       charon_string_repeated},
  {"replace",        charon_string_replace},
  {"right",          charon_string_right},
  {"sha1",           charon_string_sha1},
  {"startsWith",     charon_string_startsWith},
  {"split",          charon_string_split},
  {"suffix",         charon_string_suffix},
  {"trimmed",        charon_string_trimmed},
  {"leftTrimmed",    charon_string_leftTrimmed},
  {"rightTrimmed",   charon_string_rightTrimmed},
  {"truncate",       charon_string_truncate},
  {"underscore",     charon_string_underscore},
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
charon_StringInstanceMethodCenter( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr = (string **)lua_newuserdata(L, sizeof(string*));
  int size        = luaL_checkinteger(L, 2);
  const char *pad = luaL_checkstring(L, 3);
  *ptr = udata->center(size, pad);
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodClear( lua_State *L ) {
  string * udata  = checkString( L );
  udata->clear();
  return 0;
}

static int
charon_StringInstanceMethodContains( lua_State *L ) {
  string * udata  = checkString( L );
  const char *pad = luaL_checkstring(L, 2);
  lua_pushboolean(L, udata->contains(pad));
  return 1;
}

static int
charon_StringInstanceMethodCount( lua_State *L ) {
  string * udata  = checkString( L );
  const char *pad = luaL_checkstring(L, 2);
  lua_pushinteger(L, udata->count(pad));
  return 1;
}

static int
charon_StringInstanceMethodChop( lua_State *L ) {
  string * udata = checkString( L );
  int n = luaL_checkinteger(L, 2);
  string **ptr = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->chop(n);
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodDasherize( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->dasherize();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodEscape( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->escape();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodEscapeHtml( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->escapeHtml();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodIndexOf( lua_State *L ) {
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
charon_StringInstanceMethodInsert( lua_State *L ) {
  string * udata = checkString( L );
  int len        = luaL_checkinteger(L, 2);
  const char *ba = luaL_checkstring(L, 3);
  string **ptr = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->insert(len-1, ba);
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodEndsWith( lua_State *L ) {
  string * udata  = checkString( L );
  const char * ba = luaL_checkstring(L, 2);
  bool result     = udata->endsWith(ba);
  lua_pushboolean(L, result);
  return 1;
}

static int
charon_StringInstanceMethodLastIndexOf( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  int  result      = udata->lastIndexOf(str);
  lua_pushinteger(L, result);
  return 1;
}

static int
charon_StringInstanceMethodLeft( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  int len        = luaL_checkinteger(L, 2);
  *ptr = udata->left(len);
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodLeftJustified( lua_State *L ) {
  string * udata   = checkString( L );
  string **ptr     = (string **)lua_newuserdata(L, sizeof(string*));
  size_t size      = luaL_checkinteger(L, 2);
  const char * pad = luaL_checkstring(L, 3);
  *ptr = udata->leftJustified(size, pad);
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodMid( lua_State *L ) {
  string * udata = checkString( L );
  int pos = luaL_checkinteger(L, 2);
  int len = -1;
  if(lua_gettop(L) == 3) { // number of args
    len = luaL_checkinteger(L, 3);
  }

  string **ptr = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->mid(pos-1, len);
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodMd5( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->md5();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodNormalize( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->normalize();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodReplace( lua_State *L ) {
  size_t len1, len2;
  string * udata = checkString( L );
  const char * before = luaL_checklstring(L, 2, &len1);
  const char * after  = luaL_checklstring(L, 3, &len2);
  string * result;
  int start = 0;

  if(lua_gettop(L) > 3) { /* número de argumentos */
    start = luaL_checkinteger(L, 4);
    if( start > 0 ) {
      start = start - 1;
    }
  }

  if( len1 == 1 && len2 == 1) {
    result = udata->replace(before[0], after[0], start);
  } else {
    result = udata->replace(before, after, start);
  }
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = result;
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodRepeated( lua_State *L ) {
  string * udata = checkString( L );
  int times      = luaL_checkinteger(L, 2);
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->repeated(times);
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodReserve( lua_State *L ) {
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
charon_StringInstanceMethodRight( lua_State *L ) {
  string *  udata = checkString( L );
  int len = luaL_checkinteger(L, 2);
  string ** ptr   = (string **)lua_newuserdata(L, sizeof(string*));
          * ptr   = udata->right(len);
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodRightJustified( lua_State *L ) {
  string * udata   = checkString( L );
  string **ptr     = (string **)lua_newuserdata(L, sizeof(string*));
  size_t size      = luaL_checkinteger(L, 2);
  const char * pad = luaL_checkstring(L, 3);
  *ptr = udata->rightJustified(size, pad);
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodEncode64( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->encode64();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodDecode64( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->decode64();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodSha1( lua_State *L ) {
  string * udata = checkString( L );
  string **ptr   = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = udata->sha1();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodSimplified( lua_State *L ) {
  string *  udata = checkString( L );
  string ** ptr   = (string **)lua_newuserdata(L, sizeof(string*));
          * ptr   = udata->simplified();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodSuffix( lua_State *L ) {
  string * udata = checkString( L );
  string * result;
  if(lua_gettop(L) == 2) { // number of arguments
    const char * chr = luaL_checkstring(L, 2);
    result = udata->suffix(chr[0]);
  } else {
    result = udata->suffix();
  }
  string **ptr = (string **)lua_newuserdata(L, sizeof(string*));
  *ptr = result;
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodLeftTrimmed( lua_State *L ) {
  string *  udata = checkString( L );
  string ** ptr   = (string **)lua_newuserdata(L, sizeof(string*));
          * ptr   = udata->leftTrimmed();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodTrimmed( lua_State *L ) {
  string *  udata = checkString( L );
  string ** ptr   = (string **)lua_newuserdata(L, sizeof(string*));
          * ptr   = udata->trimmed();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodStartsWith( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  lua_pushboolean(L, udata->startsWith(str));
  return 1;
}

static int
charon_StringInstanceMethodRightTrimmed( lua_State *L ) {
  string *  udata = checkString( L );
  string ** ptr   = (string **)lua_newuserdata(L, sizeof(string*));
          * ptr   = udata->rightTrimmed();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodTruncate( lua_State *L ) {
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

  string * result = udata->truncate(pos, omission, separator);
  string ** ptr   = (string **)lua_newuserdata(L, sizeof(string*));
          * ptr   = result;
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodUnderscore( lua_State *L ) {
  string *  udata = checkString( L );
  string ** ptr   = (string **)lua_newuserdata(L, sizeof(string*));
          * ptr   = udata->underscore();
  luaL_getmetatable(L, "charon.string.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodAppend( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  udata->append(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodPrepend( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  udata->prepend(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodSplit( lua_State *L ) {
  string * udata = checkString( L );
  const char  * pattern = luaL_checkstring(L, 2);
  List * list  = udata->split(pattern);
  List **ptr   = (List **)lua_newuserdata(L, sizeof(List*));
  *ptr = list;
  luaL_getmetatable(L, "charon.string.List.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_StringInstanceMethodToString( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushlstring(L, udata->data(), udata->size());
  return 1;
}

static int
charon_StringInstanceMethodToSize( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static int
charon_StringInstanceMethodSize( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static int
charon_StringInstanceMethodLen( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushinteger(L, udata->len());
  return 1;
}

static const
luaL_reg StringInstanceMethods[] = {
  {"append",         charon_StringInstanceMethodAppend},
  {"center",         charon_StringInstanceMethodCenter},
  {"chop",           charon_StringInstanceMethodChop},
  {"clear",          charon_StringInstanceMethodClear},
  {"contains",       charon_StringInstanceMethodContains},
  {"count",          charon_StringInstanceMethodCount},
  {"dasherize",      charon_StringInstanceMethodDasherize},
  {"escape",         charon_StringInstanceMethodEscape},
  {"decode64",       charon_StringInstanceMethodDecode64},
  {"encode64",       charon_StringInstanceMethodEncode64},
  {"escapeHtml",     charon_StringInstanceMethodEscapeHtml},
  {"indexOf",        charon_StringInstanceMethodIndexOf},
  {"endsWith",       charon_StringInstanceMethodEndsWith},
  {"lastIndexOf",    charon_StringInstanceMethodLastIndexOf},
  {"left",           charon_StringInstanceMethodLeft},
  {"leftJustified",  charon_StringInstanceMethodLeftJustified},
  {"mid",            charon_StringInstanceMethodMid},
  {"insert",         charon_StringInstanceMethodInsert},
  {"len",            charon_StringInstanceMethodLen},
  {"md5",            charon_StringInstanceMethodMd5},
  {"normalize",      charon_StringInstanceMethodNormalize},
  {"prepend",        charon_StringInstanceMethodPrepend},
  {"repeated",       charon_StringInstanceMethodRepeated},
  {"replace",        charon_StringInstanceMethodReplace},
  {"reserve",        charon_StringInstanceMethodReserve},
  {"right",          charon_StringInstanceMethodRight},
  {"rightJustified", charon_StringInstanceMethodRightJustified},
  {"sha1",           charon_StringInstanceMethodSha1},
  {"size",           charon_StringInstanceMethodSize},
  {"simplified",     charon_StringInstanceMethodSimplified},
  {"suffix",         charon_StringInstanceMethodSuffix},
  {"trimmed",        charon_StringInstanceMethodTrimmed},
  {"rightTrimmed",   charon_StringInstanceMethodRightTrimmed},
  {"split",          charon_StringInstanceMethodSplit},
  {"leftTrimmed",    charon_StringInstanceMethodLeftTrimmed},
  {"truncate",       charon_StringInstanceMethodTruncate},
  {"underscore",     charon_StringInstanceMethodUnderscore},
  {"startsWith",     charon_StringInstanceMethodStartsWith},
  {"__tostring",     charon_StringInstanceMethodToString},
  {"__len",          charon_StringInstanceMethodToSize},
  {NULL, NULL}
};

void static
registerStringInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "charon.string.metatable");
  luaL_register(L, NULL, StringInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * checkList
 */

List *
checkList( lua_State *L ) {
  return *(List **) luaL_checkudata(L, 1, "charon.string.List.metatable");
}

/**
 * ClassMethods
 */

static int
charon_string_ListClassMethodNew( lua_State *L ) {
  List **ptr = (List **)lua_newuserdata(L, sizeof(List*));
  *ptr= new List();
  luaL_getmetatable(L, "charon.string.List.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg charon_string_ListClassMethods[] = {
  {"new", charon_string_ListClassMethodNew},
  {NULL, NULL}
};

void static
registerStringListClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "charon.string.List");
  luaL_register(L, NULL, charon_string_ListClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
charon_string_ListInstanceMethodDestruct( lua_State *L ) {
  List *udata = checkList( L );
  delete udata;
  return 0;
}

static int
charon_string_ListInstanceMethodAt( lua_State *L ) {
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
charon_string_list_each( lua_State *L ) {
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
charon_string_ListInstanceMethodEach( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushlightuserdata(L, udata);
  lua_pushcclosure(L, charon_string_list_each, 1);
  return 1;
}

static int
charon_string_ListInstanceMethodFirst( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushstring(L, udata->first());
  return 1;
}

static int
charon_string_ListInstanceMethodLast( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushstring(L, udata->last());
  return 1;
}

static int
charon_string_ListInstanceMethodReplace( lua_State *L ) {
  List * udata  = checkList( L );
  int pos = luaL_checkint(L, 2);
  const char * value = luaL_checkstring(L, 3);
  udata->replace(pos, value);
  return 0;
}

static int
charon_string_ListInstanceMethodJoin( lua_State *L ) {
  List * udata   = checkList( L );
  const char  * separator = luaL_checkstring(L, 2);
  char * result = udata->join(separator);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}


static int
charon_string_ListInstanceMethodAppend( lua_State *L ) {
  List * udata  = checkList( L );
  const char * value = luaL_checkstring(L, 2);
  udata->append(value);
  lua_pushvalue(L, -2);
  return 1;
}

static int
charon_string_ListInstanceMethodSize( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static const
luaL_reg charon_string_ListInstanceMethods[] = {
  {"append",  charon_string_ListInstanceMethodAppend},
  {"at",      charon_string_ListInstanceMethodAt},
  {"each",    charon_string_ListInstanceMethodEach},
  {"first",   charon_string_ListInstanceMethodFirst},
  {"last",    charon_string_ListInstanceMethodLast},
  {"join",    charon_string_ListInstanceMethodJoin},
  {"size",    charon_string_ListInstanceMethodSize},
  {"replace", charon_string_ListInstanceMethodReplace},
  {"__gc",    charon_string_ListInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerStringListInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "charon.string.List.metatable");
  luaL_register(L, NULL, charon_string_ListInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// REGISTERS
//-----------------------------------------------------------------------------

int luaopen_charon_string( lua_State *L ) {
  registerStringClassMethods(L);
  registerStringInstanceMethods(L);
  registerStringListClassMethods(L);
  registerStringListInstanceMethods(L);
  return 1;
}
