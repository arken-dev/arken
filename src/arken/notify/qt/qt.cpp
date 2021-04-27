// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/notify.h>
#include <dialog.h>

namespace arken {

void notify::send(const char * title, const char * message, const char * status)
{
  Dialog::send(new NotifyNode(status, title, message));
}

} // namespace arken
