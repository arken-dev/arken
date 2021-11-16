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
  arken_concurrent_task_fifo SHARED fifo.cpp
)

set_target_properties(arken_concurrent_task_fifo PROPERTIES OUTPUT_NAME fifo PREFIX "")

target_link_libraries(arken_concurrent_task_fifo arken lua json)
