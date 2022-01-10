#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/base>

static int
json_decode_data(lua_State *L) {
  const char * data = luaL_checkstring(L, 1);
  json_lock_decode(L, data);
  return 1;
}

static int
json_decode_file(lua_State *L) {

  const char * fileName = luaL_checkstring(L, 1);
  if( os::exists(fileName) ) {
    json_lock_decode(L, os::read(fileName));
    return 1;
  } else {
    lua_pushstring(L, string("file ").append(fileName).append(" not exists"));
    lua_error(L);
    return 0;
  }
}

static int
json_encode_data(lua_State *L) {
  char * data = json_lock_encode(L);
  lua_pushstring(L, data);
  delete[] data;
  return 1;
}

extern "C" {
  int luaopen_arken_json( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"decode", json_decode_data},
      {"encode", json_encode_data},
      {"file",   json_decode_file},
      {NULL, NULL}
    };
    luaL_newmetatable(L, "arken.json");
    luaL_register(L, NULL, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
