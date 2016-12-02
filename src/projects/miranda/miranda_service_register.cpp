#include <lua/lua.hpp>
#include "mirandaservice.h"

static int
miranda_service_loop(lua_State * L) {
  lua_getglobal( L, "__miranda_service");
  MirandaService *service = (MirandaService *) lua_touserdata(L, -1);
  int secs;
  if(lua_gettop(L) == 1 ) {
    secs = 1;
  } else {
    secs = luaL_checkint(L, 1);
  }
  lua_pushboolean(L, service->loop(secs));

  return 1;
}

void
miranda_service_register(lua_State * L) {
  static const  luaL_reg Map[] = {
    {"loop", miranda_service_loop},
    {NULL, NULL}
  };

  luaL_register(L, "service", Map);
}
