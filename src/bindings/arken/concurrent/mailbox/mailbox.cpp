// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/mailbox.h>
#include <arken/json.h>

using Mailbox = arken::concurrent::Mailbox;
using json    = arken::json;

static Mailbox *
checkMailbox( lua_State *L ) {
  return *static_cast<Mailbox **>(luaL_checkudata(L, 1, "arken.concurrent.mailbox.metatable"));
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_concurrent_mailbox_open( lua_State *L ) {
  const char * name = luaL_checkstring(L, 1);

  auto ptr = static_cast<Mailbox **>(lua_newuserdata(L, sizeof(Mailbox*)));
  *ptr = new Mailbox(name);

  luaL_getmetatable(L, "arken.concurrent.mailbox.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const luaL_reg arken_concurrent_mailbox[] = {
  {"open", arken_concurrent_mailbox_open},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_mailbox( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.mailbox");
  luaL_register(L, nullptr, arken_concurrent_mailbox);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// Instance Methods
//-----------------------------------------------------------------------------

static int
arken_concurrent_mailbox_send( lua_State *L ) {
  Mailbox * udata = checkMailbox( L );
  luaL_checkany(L, 2);
  lua_settop(L, 2);

  char * message = json::encode(L);

  try {
    udata->send(message);
  } catch (const char * msg) {
    delete[] message;
    lua_pushstring(L, msg);
    lua_error(L);
  }

  delete[] message;

  return 0;
}

static int
arken_concurrent_mailbox_receive( lua_State *L ) {
  Mailbox * udata = checkMailbox( L );

  std::optional<std::string> message = udata->receive();

  if( ! message.has_value() ) {
    lua_pushnil(L);
    return 1;
  }

  json::decode(L, message->c_str());
  return 1;
}

static int
arken_concurrent_mailbox_fetch( lua_State *L ) {
  Mailbox * udata = checkMailbox( L );

  std::optional<std::string> message = udata->fetch();

  if( ! message.has_value() ) {
    lua_pushnil(L);
    return 1;
  }

  json::decode(L, message->c_str());
  return 1;
}

static int
arken_concurrent_mailbox_isClosed( lua_State *L ) {
  Mailbox * udata = checkMailbox( L );
  lua_pushboolean(L, udata->isClosed());
  return 1;
}

static int
arken_concurrent_mailbox_close( lua_State *L ) {
  Mailbox * udata = checkMailbox( L );
  udata->close();
  return 0;
}

static int
arken_concurrent_mailbox_gc( lua_State *L ) {
  Mailbox * udata = checkMailbox( L );
  delete udata;
  return 0;
}

static const
luaL_reg arken_concurrent_mailbox_metatable[] = {
  {"send",     arken_concurrent_mailbox_send},
  {"receive",  arken_concurrent_mailbox_receive},
  {"fetch",    arken_concurrent_mailbox_fetch},
  {"isClosed", arken_concurrent_mailbox_isClosed},
  {"close",    arken_concurrent_mailbox_close},
  {"__gc",     arken_concurrent_mailbox_gc},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_mailbox_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.mailbox.metatable");
  luaL_register(L, nullptr, arken_concurrent_mailbox_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_mailbox( lua_State *L ) {
    register_arken_concurrent_mailbox_metatable(L);
    register_arken_concurrent_mailbox(L);
    return 1;
  }
}
