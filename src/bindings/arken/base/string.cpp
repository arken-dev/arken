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
  return *static_cast<string **>(luaL_checkudata(L, index, "arken.string.metatable"));
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
  auto ptr = static_cast<string **>(lua_newuserdata(L, sizeof(string*)));
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
  char * result = string::chop(string);
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
  char * result;
  if(lua_gettop(L) == 2) { /* número de argumentos */
    const char *escape = luaL_checkstring(L, 2);
     result = string::escape(string, escape[0]);
  } else {
     result = string::escape(string);
  }

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

  if(lua_gettop(L) == 3) { // number of args
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
  if(lua_gettop(L) == 3) { // number of args
    len =  luaL_checkinteger(L, 3);
  } else {
    len = -1;
  }
  char *result = string::mid(string, pos-1, len);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_normalize( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::normalize(string);
  lua_pushstring(L, result);
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
  lua_pushstring(L, result);
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
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_right( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  int    len    =  luaL_checkinteger(L, 2);
  char * result = string::right(string, len);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_squish( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result = string::squish(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_parameterize( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result = string::parameterize(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_startsWith( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * ba     = luaL_checkstring(L, 2);
  bool result         = string::startsWith(string, ba);
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_string_suffix( lua_State *L ) {
  const char * string  = luaL_checkstring(L, 1);
  const char * pattern = luaL_checkstring(L, 2);
  char * result  = string::suffix(string, pattern);;
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_split( lua_State *L ) {
  size_t len;
  const char  * string  = luaL_checklstring(L, 1, &len);
  const char  * pattern = luaL_checkstring(L, 2);
  List list = string::split(string, len, pattern);
  auto ptr  = static_cast<List **>(lua_newuserdata(L, sizeof(List*)));
  *ptr = new List(std::move(list));
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

  if(lua_gettop(L) > 3) {
    start = luaL_checkinteger(L, 4);
    if( start >= 0 ) {
      start = start - 1;
    }
  }

  char * result = string::replace(string, before, after, start);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_replaceChar( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * before = luaL_checkstring(L, 2);
  const char * after  = luaL_checkstring(L, 3);
  int start = 0;

  if(lua_gettop(L) > 3) {
    start = luaL_checkinteger(L, 4);
    if( start >= 0 ) {
      start = start - 1;
    }
  }

  char * result = string::replaceChar(string, before[0], after[0], start);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_remove( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * before = luaL_checkstring(L, 2);
  int start = 0;

  if(lua_gettop(L) > 2) {
    start = luaL_checkinteger(L, 3);
    if( start >= 0 ) {
      start = start - 1;
    }
  }

  char * result = string::remove(string, before, start);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_removeChar( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * before = luaL_checkstring(L, 2);
  int start = 0;

  if(lua_gettop(L) > 2) {
    start = luaL_checkinteger(L, 3);
    if( start >= 0 ) {
      start = start - 1;
    }
  }

  char * result = string::removeChar(string, before[0], start);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_trim( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::trim(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_trimLeft( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result       = string::trimLeft(string);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_trimRight( lua_State *L ) {
  const char * str = luaL_checkstring(L, 1);
  char * result    = string::trimRight(str);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_truncate( lua_State *L ) {
  const char *string = luaL_checkstring(L, 1);
  int pos = luaL_checkinteger(L, 2);
  const char * omission;
  char separator;

  if(lua_gettop(L) >= 3) {
    omission = lua_tostring(L, 3);
  } else {
    omission = "...";
  }

  if(lua_gettop(L) == 4) {
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
arken_string[] = {
  {"new",         arken_string_new},
  {"append",      arken_string_append},
  {"blank",       arken_string_blank},
  {"camelCase",   arken_string_camelCase},
  {"capitalize",  arken_string_capitalize},
  {"center",      arken_string_center},
  {"contains",    arken_string_contains},
  {"chop",        arken_string_chop},
  {"count",       arken_string_count},
  {"endsWith",    arken_string_endsWith},
  {"escape",      arken_string_escape},
  {"escapeHtml",  arken_string_escapeHtml},
  {"hash",        arken_string_hash},
  {"encode64",    arken_string_encode64},
  {"equals",      arken_string_equals},
  {"empty",       arken_string_empty},
  {"decode64",    arken_string_decode64},
  {"dasherize",   arken_string_dasherize},
  {"indexOf",     arken_string_indexOf},
  {"insert",      arken_string_insert},
  {"left",        arken_string_left},
  {"lastIndexOf", arken_string_lastIndexOf},
  {"md5",         arken_string_md5},
  {"mid",         arken_string_mid},
  {"normalize",   arken_string_normalize},
  {"padLeft",     arken_string_padLeft},
  {"padRight",    arken_string_padRight},
  {"prefix",      arken_string_prefix},
  {"squish",      arken_string_squish},
  {"parameterize",arken_string_parameterize},
  {"repeated",    arken_string_repeated},
  {"replace",     arken_string_replace},
  {"replaceChar", arken_string_replaceChar},
  {"remove",      arken_string_remove},
  {"removeChar",  arken_string_removeChar},
  {"right",       arken_string_right},
  {"sha1",        arken_string_sha1},
  {"startsWith",  arken_string_startsWith},
  {"split",       arken_string_split},
  {"suffix",      arken_string_suffix},
  {"trim",        arken_string_trim},
  {"trimLeft",    arken_string_trimLeft},
  {"trimRight",   arken_string_trimRight},
  {"truncate",    arken_string_truncate},
  {"underscore",  arken_string_underscore},
  {nullptr, nullptr}
};

void static
register_arken_string( lua_State *L ) {
  luaL_register(L, "string", arken_string);
}

//-----------------------------------------------------------------------------
// InstanceMethods
//-----------------------------------------------------------------------------

static int
arken_string_metatable_center( lua_State *L ) {
  string * udata  = checkString( L );
  int size        = luaL_checkinteger(L, 2);
  const char *pad = luaL_checkstring(L, 3);
  string result = udata->center(size, pad);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_clear( lua_State *L ) {
  string * udata = checkString( L );
  udata->clear();
  return 0;
}

static int
arken_string_metatable_blank( lua_State *L ) {
  string * udata = checkString( L );
  bool result = udata->blank();
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_string_metatable_camelCase( lua_State *L ) {
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
arken_string_metatable_capitalize( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->capitalize();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_contains( lua_State *L ) {
  string * udata  = checkString( L );
  const char *pad = luaL_checkstring(L, 2);
  lua_pushboolean(L, udata->contains(pad));
  return 1;
}

static int
arken_string_metatable_count( lua_State *L ) {
  string * udata  = checkString( L );
  const char *pad = luaL_checkstring(L, 2);
  lua_pushinteger(L, udata->count(pad));
  return 1;
}

static int
arken_string_metatable_chop( lua_State *L ) {
  string * udata = checkString( L );
  string result = udata->chop();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_dasherize( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->dasherize();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_empty( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushboolean(L, udata->empty());
  return 1;
}

static int
arken_string_metatable_escape( lua_State *L ) {
  string * udata = checkString( L );
  if(lua_gettop(L) == 2) { /* número de argumentos */
    const char *escape = luaL_checkstring(L, 2);
    string result  = udata->escape(escape[0]);
    lua_pushlstring(L, result.data(), result.size());
  } else {
    string result  = udata->escape();
    lua_pushlstring(L, result.data(), result.size());
  }

  return 1;
}

static int
arken_string_metatable_escapeHtml( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->escapeHtml();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_hash( lua_State *L ) {
  string * udata  = checkString( L );
  size_t   result = udata->hash();
  lua_pushnumber(L, result);
  return 1;
}

static int
arken_string_metatable_indexOf( lua_State *L ) {
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
arken_string_metatable_insert( lua_State *L ) {
  string * udata = checkString( L );
  int len        = luaL_checkinteger(L, 2);
  const char *ba = luaL_checkstring(L, 3);
  string result  = udata->insert(len-1, ba);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_endsWith( lua_State *L ) {
  string * udata  = checkString( L );
  const char * ba = luaL_checkstring(L, 2);
  lua_pushboolean(L, udata->endsWith(ba));
  return 1;
}

static int
arken_string_metatable_lastIndexOf( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  int  result      = udata->lastIndexOf(str);
  lua_pushinteger(L, result);
  return 1;
}

static int
arken_string_metatable_left( lua_State *L ) {
  string * udata = checkString( L );
  int len        = luaL_checkinteger(L, 2);
  string result  = udata->left(len);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_padLeft( lua_State *L ) {
  string * udata   = checkString( L );
  size_t size      = luaL_checkinteger(L, 2);
  const char * pad = luaL_checkstring(L, 3);
  string result    = udata->padLeft(size, pad);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_mid( lua_State *L ) {
  string * udata = checkString( L );
  int pos = luaL_checkinteger(L, 2);
  int len = -1;
  if(lua_gettop(L) == 3) {
    len = luaL_checkinteger(L, 3);
  }
  string result    = udata->mid(pos-1, len);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_md5( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->md5();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_normalize( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->normalize();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_remove( lua_State *L ) {
  string * udata = checkString( L );
  const char * before = luaL_checkstring(L, 2);
  int start = 0;

  if(lua_gettop(L) > 2) {
    start = luaL_checkinteger(L, 3);
    if( start >= 0 ) {
      start = start - 1;
    }
  }

  string result = udata->remove(before, start);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_removeChar( lua_State *L ) {
  string * udata = checkString( L );
  const char * before = luaL_checkstring(L, 2);
  int start = 0;

  if(lua_gettop(L) > 2) {
    start = luaL_checkinteger(L, 3);
    if( start >= 0 ) {
      start = start - 1;
    }
  }

  string result = udata->removeChar(before[0], start);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_replace( lua_State *L ) {
  string * udata = checkString( L );
  const char * before = luaL_checkstring(L, 2);
  const char * after  = luaL_checkstring(L, 3);
  int start = 0;

  if(lua_gettop(L) > 3) {
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
arken_string_metatable_replaceChar( lua_State *L ) {
  string * udata = checkString( L );
  const char * before = luaL_checkstring(L, 2);
  const char * after  = luaL_checkstring(L, 3);
  int start = 0;

  if(lua_gettop(L) > 3) {
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
arken_string_metatable_repeated( lua_State *L ) {
  string * udata = checkString( L );
  int times      = luaL_checkinteger(L, 2);
  string result  = udata->repeated(times);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_reserve( lua_State *L ) {
  string * udata = checkString( L );
  if(lua_gettop(L) > 1) {
   int reserve = luaL_checklong(L, 2);
    udata->reserve(reserve);
    return 0;
  } else {
    lua_pushnumber(L, udata->reserve());
    return 1;
  }
}

static int
arken_string_metatable_right( lua_State *L ) {
  string * udata = checkString( L );
  int len        = luaL_checkinteger(L, 2);
  string result  = udata->right(len) ;
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_padRight( lua_State *L ) {
  string * udata   = checkString( L );
  size_t size      = luaL_checkinteger(L, 2);
  const char * pad = luaL_checkstring(L, 3);
  string result    = udata->padRight(size, pad);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_encode64( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->encode64();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_decode64( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->decode64();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_prefix( lua_State *L ) {
  string * udata = checkString( L );
  string result;
  const char * pattern = luaL_checkstring(L, 2);
  result = udata->prefix(pattern);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_sha1( lua_State *L ) {
  string * udata = checkString( L );
  string result  = udata->sha1();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_squish( lua_State *L ) {
  string *  udata = checkString( L );
  string result = udata->squish();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_parameterize( lua_State *L ) {
  string *  udata = checkString( L );
  string result = udata->parameterize();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}


static int
arken_string_metatable_suffix( lua_State *L ) {
  string * udata = checkString( L );
  const char * chr = luaL_checkstring(L, 2);
  string result = udata->suffix(chr);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_trimLeft( lua_State *L ) {
  string *  udata = checkString( L );
  string result   = udata->trimLeft();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_trim( lua_State *L ) {
  string *  udata = checkString( L );
  string result   = udata->trim();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_startsWith( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  lua_pushboolean(L, udata->startsWith(str));
  return 1;
}

static int
arken_string_metatable_trimRight( lua_State *L ) {
  string *  udata = checkString( L );
  string result   = udata->trimRight();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_truncate( lua_State *L ) {
  string *  udata = checkString( L );
  int pos = luaL_checkinteger(L, 2);
  const char * omission;
  char separator;

  if(lua_gettop(L) >= 3) {
    omission = lua_tostring(L, 3);
  } else {
    omission = "...";
  }

  if(lua_gettop(L) == 4) {
    separator = lua_tostring(L, 4)[0];
  } else {
    separator = ' ';
  }

  string result = udata->truncate(pos, omission, separator);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_underscore( lua_State *L ) {
  string *  udata = checkString( L );
  string result   = udata->underscore();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_string_metatable_append( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  udata->append(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
arken_string_metatable_prepend( lua_State *L ) {
  string * udata   = checkString( L );
  const char * str = luaL_checkstring(L, 2);
  udata->prepend(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
arken_string_metatable_split( lua_State *L ) {
  string * udata = checkString( L );
  const char  * pattern = luaL_checkstring(L, 2);
  List list  = udata->split(pattern);
  auto ptr = static_cast<List **>(lua_newuserdata(L, sizeof(List*)));
  *ptr = new List(std::move(list));
  luaL_getmetatable(L, "arken.string.List.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_string_metatable_toString( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushlstring(L, udata->data(), udata->size());
  return 1;
}

static int
arken_string_metatable_equals( lua_State *L ) {
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
arken_string_metatable_toLen( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushinteger(L, udata->len());
  return 1;
}

static int
arken_string_metatable_size( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static int
arken_string_metatable_len( lua_State *L ) {
  string * udata = checkString( L );
  lua_pushinteger(L, udata->len());
  return 1;
}

static int
arken_string_metatable_destruct( lua_State *L ) {
  string * udata = checkString( L );
  delete udata;
  return 0;
}

static const
luaL_reg arken_string_metatable[] = {
  {"append",      arken_string_metatable_append},
  {"blank",       arken_string_metatable_blank},
  {"center",      arken_string_metatable_center},
  {"chop",        arken_string_metatable_chop},
  {"clear",       arken_string_metatable_clear},
  {"camelCase",   arken_string_metatable_camelCase},
  {"capitalize",  arken_string_metatable_capitalize},
  {"contains",    arken_string_metatable_contains},
  {"count",       arken_string_metatable_count},
  {"dasherize",   arken_string_metatable_dasherize},
  {"empty",       arken_string_metatable_empty},
  {"escape",      arken_string_metatable_escape},
  {"decode64",    arken_string_metatable_decode64},
  {"encode64",    arken_string_metatable_encode64},
  {"escapeHtml",  arken_string_metatable_escapeHtml},
  {"hash",        arken_string_metatable_hash},
  {"indexOf",     arken_string_metatable_indexOf},
  {"endsWith",    arken_string_metatable_endsWith},
  {"equals",      arken_string_metatable_equals},
  {"lastIndexOf", arken_string_metatable_lastIndexOf},
  {"left",        arken_string_metatable_left},
  {"mid",         arken_string_metatable_mid},
  {"insert",      arken_string_metatable_insert},
  {"len",         arken_string_metatable_len},
  {"md5",         arken_string_metatable_md5},
  {"normalize",   arken_string_metatable_normalize},
  {"padLeft",     arken_string_metatable_padLeft},
  {"padRight",    arken_string_metatable_padRight},
  {"prepend",     arken_string_metatable_prepend},
  {"prefix",      arken_string_metatable_prefix},
  {"repeated",    arken_string_metatable_repeated},
  {"remove",      arken_string_metatable_remove},
  {"removeChar",  arken_string_metatable_removeChar},
  {"replace",     arken_string_metatable_replace},
  {"replaceChar", arken_string_metatable_replaceChar},
  {"reserve",     arken_string_metatable_reserve},
  {"right",       arken_string_metatable_right},
  {"sha1",        arken_string_metatable_sha1},
  {"size",        arken_string_metatable_size},
  {"squish",      arken_string_metatable_squish},
  {"parameterize",arken_string_metatable_parameterize},
  {"suffix",      arken_string_metatable_suffix},
  {"split",       arken_string_metatable_split},
  {"trim",        arken_string_metatable_trim},
  {"trimLeft",    arken_string_metatable_trimLeft},
  {"trimRight",   arken_string_metatable_trimRight},
  {"truncate",    arken_string_metatable_truncate},
  {"underscore",  arken_string_metatable_underscore},
  {"startsWith",  arken_string_metatable_startsWith},
  {"__tostring",  arken_string_metatable_toString},
  {"__len",       arken_string_metatable_toLen},
  {"__gc",        arken_string_metatable_destruct},
  {nullptr, nullptr}
};

void static
register_arken_string_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.string.metatable");
  luaL_register(L, nullptr, arken_string_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * checkList
 */

List *
checkList( lua_State *L ) {
  return *static_cast<List **>(luaL_checkudata(L, 1, "arken.string.List.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_string_List_new( lua_State *L ) {
  auto ptr = static_cast<List **>(lua_newuserdata(L, sizeof(List*)));
  *ptr = new List();
  luaL_getmetatable(L, "arken.string.List.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_string_List[] = {
  {"new", arken_string_List_new},
  {nullptr, nullptr}
};

void static
register_arken_string_List( lua_State *L ) {
  luaL_newmetatable(L, "arken.string.List");
  luaL_register(L, nullptr, arken_string_List);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_string_List_gc( lua_State *L ) {
  List *udata = checkList( L );
  // check if arken_string_List_each running
  bool running = (udata->cursor() > 0 && udata->cursor() < udata->size());
  delete udata;
  if( running ) {
    const char * msg =
      "Resource closed when using \"each\" method, try to allocate a variable";
    lua_pushstring(L, msg);
    lua_error(L);
  }
  return 0;
}

static int
arken_string_List_at( lua_State *L ) {
  List * udata  = checkList( L );
  int pos = luaL_checkint(L, 2);
  int len;
  const char * str = udata->at(pos-1, &len);
  if( str == nullptr ) {
    lua_pushnil(L);
  } else {
    lua_pushlstring(L, str, len);
  }
  return 1;
}

static int
arken_string_List_closure( lua_State *L ) {
  int i = lua_upvalueindex(1);
  luaL_checktype(L, i, LUA_TLIGHTUSERDATA);
  auto ba  = static_cast<List *>(lua_touserdata(L, i));
  const char * result = ba->each();
  if ( ba->size() > 100000 ) {
    lua_pushstring(L, "achei!!!!!!!!");
    lua_error(L);
    return 0;
  }
  if( result == nullptr ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, result);
  }
  return 1;
}

static int
arken_string_List_each( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushlightuserdata(L, udata);
  lua_pushcclosure(L, arken_string_List_closure, 1);
  return 1;
}

static int
arken_string_List_first( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushstring(L, udata->first());
  return 1;
}

static int
arken_string_List_last( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushstring(L, udata->last());
  return 1;
}

static int
arken_string_List_replace( lua_State *L ) {
  List * udata  = checkList( L );
  int pos = luaL_checkint(L, 2);
  const char * value = luaL_checkstring(L, 3);
  udata->replace(pos, value);
  return 0;
}

static int
arken_string_List_join( lua_State *L ) {
  List * udata  = checkList( L );
  const char  * separator = luaL_checkstring(L, 2);
  char * result = udata->join(separator);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
arken_string_List_append( lua_State *L ) {
  List * udata  = checkList( L );
  const char * value = luaL_checkstring(L, 2);
  udata->append(value);
  lua_pushvalue(L, -2);
  return 1;
}

static int
arken_string_List_size( lua_State *L ) {
  List * udata  = checkList( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static const
luaL_reg arken_string_list_metatable[] = {
  {"append",  arken_string_List_append},
  {"at",      arken_string_List_at},
  {"each",    arken_string_List_each},
  {"first",   arken_string_List_first},
  {"last",    arken_string_List_last},
  {"join",    arken_string_List_join},
  {"size",    arken_string_List_size},
  {"replace", arken_string_List_replace},
  {"__gc",    arken_string_List_gc},
  {nullptr, nullptr}
};

void static
register_arken_string_list_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.string.List.metatable");
  luaL_register(L, nullptr, arken_string_list_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// REGISTERS
//-----------------------------------------------------------------------------

int luaopen_arken_string( lua_State *L ) {
  register_arken_string(L);
  register_arken_string_metatable(L);
  register_arken_string_List(L);
  register_arken_string_list_metatable(L);
  return 1;
}
