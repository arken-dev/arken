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
  arken.mvm SHARED mvm.cpp
)

set_target_properties(arken.mvm PROPERTIES PREFIX "" OUTPUT_NAME mvm)
target_link_libraries(arken.mvm arken)
