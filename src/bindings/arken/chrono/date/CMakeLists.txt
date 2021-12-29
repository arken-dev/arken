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
  arken.chrono.Date SHARED date.cpp
)

set_target_properties(arken.chrono.Date PROPERTIES PREFIX "" OUTPUT_NAME Date)
target_link_libraries(arken.chrono.Date arken)
