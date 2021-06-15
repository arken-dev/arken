// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <QTcpSocket>
#include <lua/lua.hpp>
#include <arken/base>
#include <arken/net/httphandle.h>
#include <task.h>

using arken::net::HttpHandle;

MirandaTask::MirandaTask(qintptr descriptor)
{
  m_descriptor  = descriptor;
}

void MirandaTask::run()
{

  QTcpSocket socket;
  QByteArray buffer;
  std::string data;
  const char * result;
  int size;

  // socket
  socket.setSocketDescriptor(m_descriptor);
  socket.waitForReadyRead(-1);
  if ( socket.bytesAvailable() < 4096 ) {
    buffer.append(socket.readAll());
  } else {
    buffer.append(socket.readAll());
    while( socket.waitForReadyRead(1000) ) {
      buffer.append(socket.readAll());
    }
  }

  // Process Request
  data   = HttpHandle::sync(buffer.data(), buffer.size());
  result = data.c_str();
  size   = data.size();

  //socket
  int bytes = 0;
  socket.write(result, size);
  socket.flush();
  while( socket.bytesToWrite() > 0 ) {
    bytes += socket.write( result+bytes, size-bytes );
    socket.flush();
    socket.waitForBytesWritten();
  }
  socket.close();
}
