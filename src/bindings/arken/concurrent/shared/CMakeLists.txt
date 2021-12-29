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
  arken.concurrent.Shared SHARED shared.cpp
)

set_target_properties(arken.concurrent.Shared PROPERTIES PREFIX "" OUTPUT_NAME Shared)
target_link_libraries(arken.concurrent.Shared arken)
