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
  arken.Image SHARED image.cpp
)

set_target_properties(arken.Image PROPERTIES PREFIX "" OUTPUT_NAME Image)
target_link_libraries(arken.Image arken)
