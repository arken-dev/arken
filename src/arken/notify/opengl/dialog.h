// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef DIALOG_H
#define DIALOG_H

class NotifyNode {
  public:
  const char * m_status;
  const char * m_title;
  const char * m_message;

  NotifyNode(const char * status, const char * title, const char * message);
};


class Dialog
{

  public:

  Dialog();
  static void send(NotifyNode * node);

};

#endif // DIALOG_H
