// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstdio>
#include <lua/lua.hpp>

#include <arken/mvm.h>
#include <arken/net/websocket.h>

namespace arken {
namespace net {

std::string WebSocketHandler::dispatcher = "arken.net.websocketDispatcher";

void
WebSocketHandler::setDispatcher(std::string dispatcher)
{
  WebSocketHandler::dispatcher = dispatcher;
}

WebSocketConnection *
WebSocketHandler::prepareCall(lua_State * L, int fd, const std::string & sessionId,
                               const std::string & path, const std::string & queryString,
                               const char * method)
{
  lua_settop(L, 0);

  // garante que a metatable de arken.net.WebSocketConnection já foi
  // registrada nessa VM antes da gente empurrar o userdata - não dá pra
  // contar com alguém ter feito isso de carona, como acontece hoje com o
  // HttpEnv (que é carregado de carona pelo dispatcher.lua)
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.net.WebSocketConnection");
  if( lua_pcall(L, 1, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
    return nullptr;
  }

  lua_getglobal(L, "require");
  lua_pushstring(L, WebSocketHandler::dispatcher.c_str());
  if( lua_pcall(L, 1, 1, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
    return nullptr;
  }

  lua_getfield(L, -1, method);

  auto ptr = static_cast<WebSocketConnection **>(lua_newuserdata(L, sizeof(WebSocketConnection*)));
  *ptr = new WebSocketConnection(fd, sessionId, path, queryString);
  luaL_getmetatable(L, "arken.net.WebSocketConnection.metatable");
  lua_setmetatable(L, -2);

  return *ptr;
}

void
WebSocketHandler::open(int fd, const std::string & sessionId, const std::string & path,
                        const std::string & queryString)
{
  mvm::instance i = mvm::getInstance();
  lua_State * L = i.state();

  if( prepareCall(L, fd, sessionId, path, queryString, "open") == nullptr ) {
    return;
  }

  if( lua_pcall(L, 1, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
  }
}

void
WebSocketHandler::message(int fd, const std::string & sessionId, const std::string & path,
                           const std::string & queryString, const std::string & payload, bool binary)
{
  mvm::instance i = mvm::getInstance();
  lua_State * L = i.state();

  if( prepareCall(L, fd, sessionId, path, queryString, "message") == nullptr ) {
    return;
  }

  lua_pushlstring(L, payload.data(), payload.size());
  lua_pushboolean(L, binary);

  if( lua_pcall(L, 3, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
  }
}

void
WebSocketHandler::close(int fd, const std::string & sessionId, const std::string & path,
                         const std::string & queryString)
{
  mvm::instance i = mvm::getInstance();
  lua_State * L = i.state();

  if( prepareCall(L, fd, sessionId, path, queryString, "close") == nullptr ) {
    return;
  }

  if( lua_pcall(L, 1, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
  }
}

void
WebSocketHandler::error(int fd, const std::string & sessionId, const std::string & path,
                         const std::string & queryString, const std::string & reason)
{
  mvm::instance i = mvm::getInstance();
  lua_State * L = i.state();

  if( prepareCall(L, fd, sessionId, path, queryString, "error") == nullptr ) {
    return;
  }

  lua_pushlstring(L, reason.data(), reason.size());

  if( lua_pcall(L, 2, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
  }
}

} // namespace net
} // namespace arken
