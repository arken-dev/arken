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
  arken.concurrent.task.priority SHARED priority.cpp
)

set_target_properties(arken.concurrent.task.priority PROPERTIES PREFIX "" OUTPUT_NAME priority)
target_link_libraries(arken.concurrent.task.priority arken)
