// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <string>

using namespace arken::net;

using arken::net::HttpBody;
using arken::net::HttpUtil;

std::string HttpHandle::sync(const char * data, size_t size)
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
  lua_getglobal(L, "dispatch");

  HttpEnv **ptr = (HttpEnv **)lua_newuserdata(L, sizeof(HttpEnv*));
  *ptr= new HttpEnv(data, size);
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
        code = 500;
        result = luaL_checklstring( L , -1, &len );
        buffer.append(result, len);
      } else {
        if( lua_isuserdata( L, 3 ) ) {
          HttpBody * body = *(HttpBody **) lua_touserdata(L, 3);
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

  return buffer;
}
