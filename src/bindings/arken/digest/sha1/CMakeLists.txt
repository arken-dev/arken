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
  arken.digest.sha1 SHARED sha1.cpp
)

set_target_properties(arken.digest.sha1 PROPERTIES PREFIX "" OUTPUT_NAME sha1)
target_link_libraries(arken.digest.sha1 arken)
