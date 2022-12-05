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
#include <arken/concurrent/service.h>

using service = arken::concurrent::service;

namespace arken {
namespace net {

string HttpServer::dispatcher = "arken.net.dispatcher";

HttpServer::HttpServer(string fileName)
{

}

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

void HttpServer::addService(string service)
{
  std::cout << "add service " << service << std::endl;
  m_service.push_back(service);
}

void HttpServer::start()
{
  if( m_service.size() > 0 ) {
    for( size_t i=0; i < m_service.size(); i++ ) {
      service::load(m_service[i]);
    }
  }

  this->run();
}

std::string HttpServer::handler(const char * data, size_t size)
{
  int code;
  size_t len;
  const char * result;
  std::string buffer;

  // arken instance
  mvm::instance i = mvm::getInstance();
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
  luaL_getmetatable(L, "arken.net.HttpEnv.metatable");
  lua_setmetatable(L, -2);

  /* error */
  if( lua_pcall(L, 1, 3, 0 ) != 0 ) {
    code = 500;
    buffer.append(HttpServer::status(code));
    buffer.append("\r\n");

    if (lua_isstring( L, -1 )) {
      result = lua_tolstring( L, -1, &len );
      buffer.append("Content-Length:");
      buffer.append(std::to_string(len));
      buffer.append("\r\n\r\n");
      buffer.append(result, len);
    } else {
      buffer.append("Content-Length: 0");
      buffer.append("\r\n\r\n");
    }

  } else {

    code = lua_tointeger( L, 1 );
    if( code < 0 ) {
      code = 500;
    }

    buffer.append(HttpServer::status(code));
    buffer.append("\r\n");

    if (lua_istable( L, 2 )) {
      for (lua_pushnil( L ); lua_next( L, 2 ); lua_pop( L, 1 )) {
        buffer.append(lua_tostring( L, -1 ));
        buffer.append("\r\n");
      }
    }

    if( ! lua_isnil( L, 3 ) ) {
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
      buffer.append("Content-Length: 0");
      buffer.append("\r\n\r\n");
    }
  }

  return std::string(std::move(buffer));
}

const char * HttpServer::status(int code)
{
  static std::map<int, const char *> list = {
    {100, "HTTP/1.1 100 Continue"},
    {101, "HTTP/1.1 101 Switching Protocols"},
    {102, "HTTP/1.1 102 Processing"},
    {200, "HTTP/1.1 200 OK"},
    {201, "HTTP/1.1 201 Created"},
    {202, "HTTP/1.1 202 Accepted"},
    {203, "HTTP/1.1 203 Non-Authoritative Information"},
    {204, "HTTP/1.1 204 No Content"},
    {205, "HTTP/1.1 205 Reset Content"},
    {206, "HTTP/1.1 206 Partial Content"},
    {207, "HTTP/1.1 207 Multi-Status"},
    {208, "HTTP/1.1 208 Already Reported"},
    {226, "HTTP/1.1 226 IM Used"},
    {300, "HTTP/1.1 300 Multiple Choices"},
    {301, "HTTP/1.1 301 Moved Permanently"},
    {302, "HTTP/1.1 302 Found"},
    {303, "HTTP/1.1 303 See Other"},
    {304, "HTTP/1.1 304 Not Modified"},
    {305, "HTTP/1.1 305 Use Proxy"},
    {306, "HTTP/1.1 306 Reserved"},
    {307, "HTTP/1.1 307 Temporary Redirect"},
    {308, "HTTP/1.1 308 Permanent Redirect"},
    {400, "HTTP/1.1 400 Bad Request"},
    {401, "HTTP/1.1 401 Unauthorized"},
    {402, "HTTP/1.1 402 Payment Required"},
    {403, "HTTP/1.1 403 Forbidden"},
    {404, "HTTP/1.1 404 Not Found"},
    {405, "HTTP/1.1 405 Method Not Allowed"},
    {406, "HTTP/1.1 406 Not Acceptable"},
    {407, "HTTP/1.1 407 Proxy Authentication Required"},
    {408, "HTTP/1.1 408 Request Timeout"},
    {409, "HTTP/1.1 409 Conflict"},
    {410, "HTTP/1.1 410 Gone"},
    {411, "HTTP/1.1 411 Length Required"},
    {412, "HTTP/1.1 412 Precondition Failed"},
    {413, "HTTP/1.1 413 Payload Too Large"},
    {414, "HTTP/1.1 414 URI Too Long"},
    {415, "HTTP/1.1 415 Unsupported Media Type"},
    {416, "HTTP/1.1 416 Range Not Satisfiable"},
    {417, "HTTP/1.1 417 Expectation Failed"},
    {418, "HTTP/1.1 418 I'm a teapot"},
    {421, "HTTP/1.1 421 Misdirected Request"},
    {422, "HTTP/1.1 422 Unprocessable Entity"},
    {423, "HTTP/1.1 423 Locked"},
    {424, "HTTP/1.1 424 Failed Dependency"},
    {426, "HTTP/1.1 426 Upgrade Required"},
    {428, "HTTP/1.1 428 Precondition Required"},
    {429, "HTTP/1.1 429 Too Many Requests"},
    {431, "HTTP/1.1 431 Request Header Fields Too Large"},
    {451, "HTTP/1.1 451 Unavailable For Legal Reasons"},
    {500, "HTTP/1.1 500 Internal Server Error"},
    {501, "HTTP/1.1 501 Not Implemented"},
    {502, "HTTP/1.1 502 Bad Gateway"},
    {503, "HTTP/1.1 503 Service Unavailable"},
    {504, "HTTP/1.1 504 Gateway Time-out"},
    {505, "HTTP/1.1 505 HTTP Version Not Supported"},
    {506, "HTTP/1.1 506 Variant Also Negotiates"},
    {507, "HTTP/1.1 507 Insufficient Storage"},
    {508, "HTTP/1.1 508 Loop Detected"},
    {510, "HTTP/1.1 510 Not Extended"},
    {511, "HTTP/1.1 511 Network Authentication Required"}
  };

  return list[code];
}

} // namespace net
} // namespace arken
