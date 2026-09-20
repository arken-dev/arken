# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

add_library(
  arken.security.RateLimit SHARED ratelimit.cpp
)

set_target_properties(arken.security.RateLimit PROPERTIES PREFIX "" OUTPUT_NAME RateLimit)
target_link_libraries(arken.security.RateLimit arken)
