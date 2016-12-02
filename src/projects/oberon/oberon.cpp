// Copyright 2016 The Oberon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <oberon/helper>

void Oberon::args(lua_State * L, int argc, char * argv[])
{

  int top, i;

  lua_newtable(L);
  top = lua_gettop(L);
  for(i=1; i <= argc; i++) {
    lua_pushinteger(L, i-1);
    lua_pushstring(L, argv[i]);
    lua_settable(L, top);
  }
  lua_setglobal( L, "arg");

}

void Oberon::profile(lua_State * L, const char * path)
{
  int rv, flag = 0, len, i;
  char * profile;
  char * oberonPath;

  len = strlen(path);

  for(i = len; i > 0; i--) {
    if( path[i] == '/' ) {
      flag++;
      if( flag == 2 ) {
        break;
      }
    }
  }

  oberonPath = string::left(path, i);

  if( strcmp(os::name(), "windows") == 0 ) {
    oberonPath[0] = toupper(oberonPath[0]);
  }

  //OBERON PATH
  lua_pushstring(L, oberonPath);
  lua_setglobal(L, "OBERON_PATH");

  //PROFILE
  profile = string::append(oberonPath, "/profile.lua");

  rv = luaL_loadfile(L, profile);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
    throw;
  }

  rv = lua_pcall(L, 0, 0, lua_gettop(L) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
    throw;
  }

  delete[] oberonPath;
  delete[] profile;
}

lua_State * Oberon::init(int argc, char * argv[], const char * path)
{

  lua_State *L = lua_open();
  if( L == 0 ) {
    return 0;
  }
  luaL_openlibs(L);

  Oberon::args(L, argc, argv);
  Oberon::profile(L, path);

  return L;
}
