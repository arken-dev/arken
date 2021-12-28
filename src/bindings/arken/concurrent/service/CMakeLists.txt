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
  arken_concurrent_service SHARED service.cpp
)

set_target_properties(arken_concurrent_service PROPERTIES OUTPUT_NAME service PREFIX "")

target_link_libraries(arken_concurrent_service arken)
