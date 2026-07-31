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
  arken.crypto.credential SHARED credential.cpp
)

set_target_properties(arken.crypto.credential PROPERTIES PREFIX "" OUTPUT_NAME credential)
target_link_libraries(arken.crypto.credential arken)
