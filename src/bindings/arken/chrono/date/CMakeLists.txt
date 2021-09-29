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
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)

add_library(
  arken.chrono.Date SHARED date.cpp
)

set_target_properties(arken.chrono.Date PROPERTIES LIBRARY_OUTPUT_NAME "Date" PREFIX "")
target_link_libraries(arken.chrono.Date arken lua)
