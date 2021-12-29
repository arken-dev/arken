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
  arken.odebug SHARED odebug.cpp
)

set_target_properties(arken.odebug PROPERTIES PREFIX "" OUTPUT_NAME odebug)
target_link_libraries(arken.odebug arken)
