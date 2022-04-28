#include <lua/lua.hpp>
#include <lua/json/lock.h>
#include <arken/base>
#include <arken/json.h>

using arken::json;

static int
arken_json_decode(lua_State *L) {
  const char * data = luaL_checkstring(L, 1);
  json::decode(L, data);
  return 1;
}

static int
arken_json_file(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  if( os::exists(fileName) ) {
    json::decode(L, os::read(fileName));
    return 1;
  } else {
    lua_pushstring(L, string("file ").append(fileName).append(" not exists"));
    lua_error(L);
    return 0;
  }
}

static int
arken_json_encode(lua_State *L) {
  char * data = json::encode(L);
  lua_pushstring(L, data);
  delete[] data;
  return 1;
}

extern "C" {
  int luaopen_arken_json( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"decode", arken_json_decode},
      {"encode", arken_json_encode},
      {"file",   arken_json_file},
      {nullptr, nullptr}
    };
    luaL_newmetatable(L, "arken.json");
    luaL_register(L, nullptr, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
