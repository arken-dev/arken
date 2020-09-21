// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Using libcurl for http request
//  https://curl.haxx.se/libcurl/c/example.html

#ifndef _ARKEN_HTTP_
#define _ARKEN_HTTP_

namespace arken
{

class http {
  public:
  static char * read(const char * url);
  static char * get(const char * url);
  static char * post(const char * url);
  static char * put(const char * url);
};

}

#endif
