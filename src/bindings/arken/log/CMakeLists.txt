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
  arken.Log SHARED log.cpp
)

set_target_properties(arken.Log PROPERTIES PREFIX "" OUTPUT_NAME Log)
target_link_libraries(arken.Log arken)
