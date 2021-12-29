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
  arken.base64 SHARED base64.cpp
)

set_target_properties(arken.base64 PROPERTIES PREFIX "" OUTPUT_NAME base64)
target_link_libraries(arken.base64 arken)
