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

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/arken/concurrent/task)

add_library(
  arken_concurrent_task_singular SHARED singular.cpp
)

find_package(Qt5Core)
find_package(Qt5Network)

set_target_properties(arken_concurrent_task_singular PROPERTIES OUTPUT_NAME singular PREFIX "")

target_link_libraries(arken_concurrent_task_singular Qt5::Core Qt5::Network arken lua json)
