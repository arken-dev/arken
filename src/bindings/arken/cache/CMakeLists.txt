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
  arken.cache SHARED cache.cpp
)

set_target_properties(arken.cache PROPERTIES PREFIX "" OUTPUT_NAME cache)
target_link_libraries(arken.cache arken)
