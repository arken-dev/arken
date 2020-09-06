// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_NOTIFY_
#define _CHARON_NOTIFY_

#include <charon/base>

using string = charon::string;

namespace charon
{

class notify {
  public:
  static void send(string title, string body, string icon = "ok");
};

}

#endif
