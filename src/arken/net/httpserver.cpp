// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <string>
#include <lua/lua.hpp>
#include <arken/os.h>
#include <arken/mvm.h>
#include <arken/net/httpserver.h>
#include <arken/net/httpbody.h>
#include <arken/net/httpenv.h>
#include <arken/net/httputil.h>

namespace arken {
namespace net {

string HttpServer::dispatcher = "dispatcher";

HttpServer::HttpServer(const char * address, int port)
{
  m_address = address;
  m_port    = port;
  m_threads = os::cores();
}

void HttpServer::setThreads(int threads)
{
  m_threads = threads;
}

void HttpServer::setPid(const char * pid)
{
  m_pid = pid;
}

void HttpServer::setDispatcher(string dispatcher)
{
  HttpServer::dispatcher = dispatcher;
}


std::string HttpServer::handler(const char * data, size_t size)
{
  int code;
  size_t len;
  const char * result;
  std::string buffer;

  // arken instance
  arken::instance i = mvm::instance();
  lua_State * L = i.state();

  // Process Request
  // TODO return is not validate
  lua_settop(L, 0);
  lua_getglobal(L, "require");
  lua_pushstring(L, HttpServer::dispatcher);
  int rv = lua_pcall(L, 1, 1, 0);
  if (rv) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
  }

  auto ptr = static_cast<HttpEnv **>(lua_newuserdata(L, sizeof(HttpEnv*)));
  *ptr = new HttpEnv(data, size);
  luaL_getmetatable(L, "HttpEnv.metatable");
  lua_setmetatable(L, -2);

  /* error */
  if( lua_pcall(L, 1, 3, 0 ) != 0 ) {
    code = 500;
    buffer.append(HttpUtil::status(code));
    buffer.append("\r\n");

    if (lua_isstring( L, -1 )) {
      result = lua_tolstring( L, -1, &len );
      buffer.append("Content-Length:");
      buffer.append(std::to_string(len));
      buffer.append("\r\n\r\n");
      buffer.append(result, len);
    }

  } else {

    code = lua_tointeger( L, 1 );
    if( code < 0 ) {
      code = 500;
    }

    buffer.append(HttpUtil::status(code));
    buffer.append("\r\n");

    if (lua_istable( L, 2 )) {
      for (lua_pushnil( L ); lua_next( L, 2 ); lua_pop( L, 1 )) {
        buffer.append(lua_tostring( L, -1 ));
        buffer.append("\r\n");
      }
    }

    if( ! lua_isnil(L, 3) ) {
     if( code < 0 ) {
        result = luaL_checklstring( L , -1, &len );
        buffer.append(result, len);
      } else {
        if( lua_isuserdata( L, 3 ) ) {
          HttpBody * body = *static_cast<HttpBody **> (lua_touserdata(L, 3));
          buffer.append("Content-Length:");
          buffer.append(std::to_string(body->size()));
          buffer.append("\r\n\r\n");
          buffer.append(body->read(), body->size());
          body->release();
        } else {
          result = luaL_checklstring( L, 3, &len );
          buffer.append("Content-Length:");
          buffer.append(std::to_string(len));
          buffer.append("\r\n\r\n");
          buffer.append(result, len);
        }
      }
    } else {
      buffer.append("\r\n");
    }
  }

  return std::move(buffer);
}


} // namespace net
} // namespace arken