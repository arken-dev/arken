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
  arken.concurrent.channel SHARED channel.cpp
)

set_target_properties(arken.concurrent.channel PROPERTIES PREFIX "" OUTPUT_NAME channel)
target_link_libraries(arken.concurrent.channel arken)
