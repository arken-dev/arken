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
  arken.concurrent.Lock SHARED lock.cpp
)

set_target_properties(arken.concurrent.Lock PROPERTIES PREFIX "" OUTPUT_NAME Lock)
target_link_libraries(arken.concurrent.Lock arken)
