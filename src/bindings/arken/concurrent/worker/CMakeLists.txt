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
  arken.concurrent.worker SHARED worker.cpp
)

set_target_properties(arken.concurrent.worker PROPERTIES PREFIX "" OUTPUT_NAME worker)
target_link_libraries(arken.concurrent.worker arken)
