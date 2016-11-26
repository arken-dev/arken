#include <luajit-2.0/lua.hpp>
#include <OByteArray>
#include <QDebug>
#include <QThread>
#include <oberon/helper>
#include <triton.h>

static int
triton_ok(lua_State *L) {
  lua_pushinteger(L, Triton::ok());
  return 1;
}

static int
triton_error(lua_State *L) {
  lua_pushinteger(L, Triton::error());
  return 1;
}

static int
triton_pending(lua_State *L) {
  lua_pushinteger(L, Triton::pending());
  return 1;
}

static int
triton_addOk(lua_State *) {
  Triton::addOk();
  return 0;
}

static int
triton_addError(lua_State *) {
  Triton::addError();
  return 0;
}

static int
triton_addPending(lua_State *) {
  Triton::addPending();
  return 0;
}


static int
triton_appendResult(lua_State *L) {
  const char * result = luaL_checkstring(L, 1);
  Triton::appendResult(result);
  return 0;
}

static int
triton_enqueue(lua_State *L) {
  const char * path = luaL_checkstring(L, 1);
  Triton::enqueue(path);
  return 0;
}

static int
triton_result(lua_State *L) {
  lua_pushstring(L, *Triton::result());
  return 1;
}


void
triton_register(lua_State * L) {
  static const luaL_reg Map[] = {
    {"ok",         triton_ok},
    {"failure",    triton_error},
    {"pending",    triton_pending},
    {"addOk",      triton_addOk},
    {"addError",   triton_addError},
    {"addPending", triton_addPending},
    {"appendResult", triton_appendResult},
    {"enqueue",    triton_enqueue},
    {"result",     triton_result},
    {NULL, NULL}
  };

  luaL_register(L, "triton", Map);
}
