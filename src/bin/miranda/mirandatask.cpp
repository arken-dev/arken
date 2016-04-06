#include <luajit-2.0/lua.hpp>
#include "mirandatask.h"
#include <iostream>
#include <QThread>

//QStack<lua_State *> * stack

MirandaTask::MirandaTask(MirandaServer * server, qintptr descriptor)
{

  m_descriptor  = descriptor;
  m_oberonPath  = server->oberonPath();
  m_profilePath = server->profilePath();

}

void MirandaTask::run()
{
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

  //lua state
  lua_State * m_State = luaL_newstate();
  luaL_openlibs(m_State);
  lua_pushstring(m_State, m_oberonPath.data());
  lua_setglobal(m_State, "OBERON_PATH");


  int rv = luaL_loadfile(m_State, m_profilePath);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  rv = lua_pcall(m_State, 0, 0, lua_gettop(m_State) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  luaL_loadfile(m_State, "process_http.lua" );
  lua_pcall(m_State, 0, 0, 0);
  qDebug() << "constructor MirandaTask";


  // Parse Request
  this->parseRequest(m_State, buffer);

  // Process Request
  this->processRequest(m_State, socket);
}

void MirandaTask::processRequest(lua_State * m_State, QTcpSocket &socket)
{

  int code;
  size_t len;
  const char * result;

  QByteArray buffer;

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
  /*
  buffer.append("HTTP/1.1 200 OK\r\n");
  buffer.append("Content-Type: text/html; charset=utf-8\r\n");
  buffer.append("Content-Length: 11\r\n\r\n");
  buffer.append("hello world\r\n");
  */
  socket.write(buffer);
  socket.flush();
  socket.close();
}


void MirandaTask::disconnected()
{

}

void MirandaTask::parseRequest(lua_State * m_State, QByteArray &buffer)
{
  int index = 0;
  int last  = 0;
  int nrec = 0;
  int method = 0;
  int tmp = 0;
  QByteArray row;
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
