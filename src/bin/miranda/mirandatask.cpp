#include <luajit-2.0/lua.hpp>
#include "mirandatask.h"
#include <iostream>
#include <QThread>
#include <QMutex>
#include <OHttpRequest>

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
  OHttpRequest * http_request = new OHttpRequest(buffer);
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
    {200, "HTTP/1.1 200 OK"},
    {301, "HTTP/1.1 301 Moved Permanently"},
    {302, "HTTP/1.1 302 Moved Temporary"},
    {404, "HTTP/1.1 404 Not Found"},
    {500, "HTTP/1.1 500 Internal Server Error"},
    {502, "HTTP/1.1 502 Bad Gateway"}
  };

  return list[code];
}
