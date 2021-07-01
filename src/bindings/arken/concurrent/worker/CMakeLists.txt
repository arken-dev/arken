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
  arken_concurrent_worker SHARED worker.cpp
)

find_package(Qt5Core)
find_package(Qt5Network)

set_target_properties(arken_concurrent_worker PROPERTIES OUTPUT_NAME worker PREFIX "")

target_link_libraries(arken_concurrent_worker Qt5::Core Qt5::Network arken lua json)
