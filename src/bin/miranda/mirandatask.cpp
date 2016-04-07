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
  lua_State * m_State;

  m_State = state->instance();
  buffer.clear();

  // Process Request
  lua_settop(m_State, 0);
  lua_getglobal(m_State, "process_http");
  if( lua_pcall(m_State, 0, 3, 0 ) ) {
    code   = 500;
    result = luaL_checklstring( m_State , -1, &len );
  } else {
    code   = lua_tointeger( m_State, 1 );
    result = luaL_checklstring( m_State, 3, &len );
  }

  buffer.append(httpStatus(code));
  buffer.append("\r\n");

  if (lua_istable( m_State, 2 )) {
    for (lua_pushnil( m_State ); lua_next( m_State, 2); lua_pop( m_State, 1 )) {
      buffer.append(lua_tostring( m_State, -1 ));
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
  QByteArray row;
  lua_State * m_State;

  m_State = state->instance();

  nrec = buffer.count("\r\n") + 1;

  //lua table
  lua_createtable(m_State, 0, nrec);

  //row
  last   = buffer.indexOf("\r\n", index);
  row    = buffer.mid(index, last-index);
  index  = last + 2;
  method = row.indexOf(' ');

  //Method
  lua_pushstring(m_State, "Method");
  lua_pushstring(m_State, row.mid(0, method));
  lua_settable(m_State, -3);

  //Query-String
  lua_pushstring(m_State, "Query-String");
  lua_pushstring(m_State, row.mid(method+3, row.lastIndexOf(' ')-(method+3)));
  lua_settable(m_State, -3);

  while( index < buffer.size() ) {
    last = buffer.indexOf("\r\n", index);
    row = buffer.mid(index, last-index);
    index = last + 2;
    tmp = row.indexOf(":");
    lua_pushstring(m_State, row.mid(0, tmp));
    lua_pushstring(m_State, row.mid(tmp+2, row.size()));
    lua_settable(m_State, -3);
  }
  lua_setglobal(m_State, "request");
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
