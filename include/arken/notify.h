// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NOTIFY_
#define _ARKEN_NOTIFY_

#include <arken/string.h>

using string = arken::string;

namespace arken
{

class notify {
  public:
  static void send(string title, string body, string icon = "ok");
};

}

#endif
