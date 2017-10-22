// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <mirandatask.h>
#include <QTcpSocket>

using charon::net::HttpHandle;

MirandaTask::MirandaTask(qintptr descriptor)
{
  m_descriptor  = descriptor;
}

void MirandaTask::run()
{

  QTcpSocket socket;
  QByteArray buffer;
  int size;
  char * result;

  // socket
  socket.setSocketDescriptor(m_descriptor);

  if ( socket.waitForReadyRead(-1) ) {
    buffer.append(socket.readAll());
  }

  // Process Request
  result = HttpHandle::sync(buffer.data(), buffer.size(), &size);

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
  delete[] result;
}
