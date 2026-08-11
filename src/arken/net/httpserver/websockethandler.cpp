// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstdio>
#include <string>
#include <lua/lua.hpp>

#include <arken/mvm.h>
#include <arken/net/httpserver.h>
#include <arken/net/websocket.h>

namespace arken {
namespace net {

std::string WebSocketHandler::dispatcher = "arken.net.websocketDispatcher";

// monta a resposta HTTP completa (status+headers+body) de recusa do
// handshake - headersIndex/bodyIndex são índices absolutos na stack Lua
// (0 = não tem, usa resposta genérica). Segue a mesma convenção de
// HttpServer::handler() (httpserver.cpp): headers é um array de linhas
// "Nome: valor" já formatadas (não um mapa), body é string simples.
static std::string
buildHandshakeReject(int code, lua_State * L, int headersIndex, int bodyIndex)
{
  const char * statusLine = HttpServer::status(code);
  if( statusLine == nullptr ) {
    statusLine = HttpServer::status(500);
  }

  std::string response(statusLine);
  response.append("\r\nConnection: close\r\n");

  if( L != nullptr && lua_istable(L, headersIndex) ) {
    for( lua_pushnil(L); lua_next(L, headersIndex); lua_pop(L, 1) ) {
      if( lua_isstring(L, -1) ) {
        response.append(lua_tostring(L, -1));
        response.append("\r\n");
      }
    }
  }

  size_t len = 0;
  const char * body = "";
  if( L != nullptr && lua_isstring(L, bodyIndex) ) {
    body = lua_tolstring(L, bodyIndex, &len);
  }

  response.append("Content-Length: ");
  response.append(std::to_string(len));
  response.append("\r\n\r\n");
  if( len > 0 ) {
    response.append(body, len);
  }

  return response;
}

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

std::string
WebSocketHandler::handshake(int fd, const std::string & sessionId, const std::string & path,
                             const std::string & queryString)
{
  mvm::instance i = mvm::getInstance();
  lua_State * L = i.state();

  if( prepareCall(L, fd, sessionId, path, queryString, "handshake") == nullptr ) {
    return buildHandshakeReject(500, nullptr, 0, 0);
  }

  // dispatcher customizado (via setDispatcher) pode não implementar
  // handshake - nesse caso mantém o comportamento antigo (sempre aceita)
  if( ! lua_isfunction(L, 2) ) {
    return "";
  }

  if( lua_pcall(L, 1, 3, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
    return buildHandshakeReject(500, nullptr, 0, 0);
  }

  // pcall(1, 3, 0) consome [method, connection] e empurra os 3 retornos
  // de object:handshake() no lugar - dispatcherTable (índice 1) continua
  // embaixo, intocado; code/headers/body ficam nos índices 2/3/4
  int code = lua_isnumber(L, 2) ? static_cast<int>(lua_tointeger(L, 2)) : 200;
  if( code == 200 ) {
    return ""; // headers/body de um 200 não têm onde ir (resposta do
               // upgrade é fixa pela RFC) - descartados de propósito
  }

  return buildHandshakeReject(code, L, 3, 4);
}

bool
WebSocketHandler::open(int fd, const std::string & sessionId, const std::string & path,
                        const std::string & queryString)
{
  mvm::instance i = mvm::getInstance();
  lua_State * L = i.state();

  if( prepareCall(L, fd, sessionId, path, queryString, "open") == nullptr ) {
    return false;
  }

  if( lua_pcall(L, 1, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
    return false;
  }

  return true;
}

bool
WebSocketHandler::message(int fd, const std::string & sessionId, const std::string & path,
                           const std::string & queryString, const std::string & payload, bool binary)
{
  mvm::instance i = mvm::getInstance();
  lua_State * L = i.state();

  if( prepareCall(L, fd, sessionId, path, queryString, "message") == nullptr ) {
    return false;
  }

  lua_pushlstring(L, payload.data(), payload.size());
  lua_pushboolean(L, binary);

  if( lua_pcall(L, 3, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
    return false;
  }

  return true;
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
