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
  arken.digest.md5 SHARED md5.cpp
)

set_target_properties(arken.digest.md5 PROPERTIES PREFIX "" OUTPUT_NAME md5)
target_link_libraries(arken.digest.md5 arken)
