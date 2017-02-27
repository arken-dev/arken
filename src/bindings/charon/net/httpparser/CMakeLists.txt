# ------------------------------------------------------------------------------
#
# Copyright 2016 The Charon Platform Authors.
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
  HttpParser SHARED httpparser.cpp
)

find_package(Qt5Core)
find_package(Qt5Network)

set_target_properties(HttpParser PROPERTIES PREFIX "")

target_link_libraries(HttpParser Qt5::Core Qt5::Network charon lua)
