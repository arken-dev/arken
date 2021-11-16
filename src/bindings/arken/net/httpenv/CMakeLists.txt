# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

include_directories(.)

add_library(
  HttpEnv SHARED httpenv.cpp
)

set_target_properties(HttpEnv PROPERTIES PREFIX "")

target_link_libraries(HttpEnv arken lua)
