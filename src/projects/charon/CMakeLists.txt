# ------------------------------------------------------------------------------
#
# Copyright 2016 The Charon Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)

add_executable( charon-bin main.cpp charon.cpp )
set_target_properties(charon-bin PROPERTIES OUTPUT_NAME charon)

find_package(Qt5Core)
find_package(Qt5Network)

target_link_libraries(charon-bin Qt5::Core lua charon curl)
target_link_libraries(charon-bin Qt5::Network)
