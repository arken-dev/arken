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
  arken.concurrent.service SHARED service.cpp
)

set_target_properties(arken.concurrent.service PROPERTIES PREFIX "" OUTPUT_NAME service)
target_link_libraries(arken.concurrent.service arken)
