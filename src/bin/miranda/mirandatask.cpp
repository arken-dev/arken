#include <luajit-2.0/lua.hpp>
#include "mirandatask.h"
#include <iostream>
#include <QThread>
#include <QMutex>

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

  // stack pop lua state
  state = MirandaState::pop();

  // Parse Request
  this->parseRequest(state, buffer);

  // Process Request
  this->processRequest(state, buffer);

  // stack push lua state
  MirandaState::push(state);

  //socket
  socket.write(buffer);
  socket.flush();
  socket.close();
}

void MirandaTask::processRequest(MirandaState * state, QByteArray &buffer)
{
  int code;
  size_t len;
  const char * result;
  lua_State * L;

  L = state->instance();
  buffer.clear();

  // Process Request
  lua_settop(L, 0);
  lua_getglobal(L, "dispatcher");
  if( lua_pcall(L, 0, 3, 0 ) ) {
    code   = 500;
    result = luaL_checklstring( L , -1, &len );
  } else {
    code   = lua_tointeger( L, 1 );
    result = luaL_checklstring( L, 3, &len );
  }

  buffer.append(httpStatus(code));
  buffer.append("\r\n");

  if (lua_istable( L, 2 )) {
    for (lua_pushnil( L ); lua_next( L, 2 ); lua_pop( L, 1 )) {
      buffer.append(lua_tostring( L, -1 ));
      buffer.append("\r\n");
    }
  }

  buffer.append("Content-Length:");
  buffer.append(QByteArray::number((int)len, 10));
  buffer.append("\r\n\r\n");
  buffer.append(result, len);
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
  lua_pushstring(L, row.mid(method+1, query-method-1));
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
    {404, "HTTP/1.1 404 Not Found"},
    {500, "HTTP/1.1 500 Internal Server Error"},
    {502, "HTTP/1.1 502 Bad Gateway"}
  };

  return list[code];
}
