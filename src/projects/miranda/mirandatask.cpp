// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/helper>
#include "mirandatask.h"
#include <iostream>
#include <QThread>
#include <QMutex>

using charon::net::HttpParser;

MirandaTask::MirandaTask(qintptr descriptor)
{
  m_descriptor  = descriptor;
}

void MirandaTask::run()
{
  MirandaState * state;
  QTcpSocket socket;
  QByteArray buffer;

  // socket
  socket.setSocketDescriptor(m_descriptor);

  //if ( socket.waitForConnected(-1) ) {
  //  qDebug() << "connect !!!";
  //}

  if ( socket.waitForReadyRead(-1) ) {
    buffer = socket.readAll();
  }
  //std::cout << buffer.data();

  // stack pop lua state
  state = MirandaState::pop();

  // Parse Request
  //this->parseRequest(state, buffer);

  // Process Request
  this->processRequest(state, buffer);

  // stack push lua state
  MirandaState::push(state);

  //socket
  int bytes = 0;
  socket.write(buffer.data(), buffer.size());
  socket.flush();
  while( socket.bytesToWrite() > 0 ) {
    bytes += socket.write( buffer.data()+bytes, buffer.size()-bytes );
    socket.flush();
    socket.waitForBytesWritten();
  }
  socket.close();
}

void MirandaTask::processRequest(MirandaState * state, QByteArray &buffer)
{
  int code;
  size_t len;
  const char * result;
  lua_State * L;

  L = state->instance();

  //request
  HttpParser * http_request = new HttpParser(buffer);
  lua_pushlightuserdata(L, http_request);
  lua_setglobal(L, "__http_request");

  //return
  buffer.clear();

  // Process Request
  // TODO return is not validate
  lua_settop(L, 0);
  lua_getglobal(L, "dispatch");

  /* error */
  if( lua_pcall(L, 0, 3, 0 ) != 0 ) {
    code = 500;
    buffer.append(httpStatus(code));
    buffer.append("\r\n");

    if (lua_istable( L, -1 )) {

    }

    if (lua_isstring( L, -1 )) {
      result = lua_tolstring( L, -1, &len );
      buffer.append("Content-Length:");
      buffer.append(QByteArray::number(((int)len), 10));
      buffer.append("\r\n\r\n");
      buffer.append(result, len);
    }

  } else {

    code = lua_tointeger( L, 1 );

    buffer.append(httpStatus(code));
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
      } else {
        result = luaL_checklstring( L, 3, &len );
        buffer.append("Content-Length:");
        buffer.append(QByteArray::number((int)len, 10));
        buffer.append("\r\n\r\n");
      }
      buffer.append(result, len);
    } else {
      buffer.append("\r\n");
    }
  }
  delete http_request;
}

void MirandaTask::parseRequest(MirandaState *state, QByteArray &buffer)
{
  int index  = 0;
  int last   = 0;
  int nrec   = 0;
  int method = 0;
  int tmp    = 0;
  int query  = 0;
  QByteArray row;
  lua_State * L;

  L = state->instance();

  nrec = buffer.count("\r\n") + 1;

  //lua table
  lua_createtable(L, 0, nrec);

  //row
  last   = buffer.indexOf("\r\n", index);
  row    = buffer.mid(index, last-index);
  index  = last + 2;
  method = row.indexOf(' ');
  query  = row.indexOf('?');

  //Method
  lua_pushstring(L, "Method");
  lua_pushstring(L, row.mid(0, method));
  lua_settable(L, -3);

  //Path
  lua_pushstring(L, "Path");
  if( row.contains("?") ) {
    lua_pushstring(L, row.mid(method+1, query-method-1));
  } else {
    lua_pushstring(L, row.mid(method+1, row.lastIndexOf(' ')-method-1));
  }
  lua_settable(L, -3);

  //Query-String
  lua_pushstring(L, "Query-String");
  lua_pushstring(L, row.mid(query+1, row.lastIndexOf(' ')-query-1));
  lua_settable(L, -3);

  while( index < buffer.size() ) {
    last  = buffer.indexOf("\r\n", index);
    row   = buffer.mid(index, last-index);
    index = last + 2;
    tmp   = row.indexOf(":");
    lua_pushstring(L, row.mid(0, tmp));
    lua_pushstring(L, row.mid(tmp+2, row.size()));
    lua_settable(L, -3);
  }
  lua_setglobal(L, "request");
}

QByteArray MirandaTask::httpStatus(int code)
{
  static QHash<int, QByteArray> list = {
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
