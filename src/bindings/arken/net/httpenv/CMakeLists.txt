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
  arken.net.HttpEnv SHARED httpenv.cpp
)

set_target_properties(arken.net.HttpEnv PROPERTIES PREFIX "" OUTPUT_NAME HttpEnv)
target_link_libraries(arken.net.HttpEnv arken)
