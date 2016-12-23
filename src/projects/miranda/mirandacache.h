// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef MIRANDACACHE_H
#define MIRANDACACHE_H

class MirandaCache
{

public:
  MirandaCache(const char * data, int expires);
  ~MirandaCache();
  const char * data();
  bool isExpires();

private:
  const char * m_data;
  double m_expires;

};

#endif // MIRANDACACHE_H
