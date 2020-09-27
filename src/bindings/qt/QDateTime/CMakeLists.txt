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
  QDateTime SHARED QDateTime.cpp
)

find_package(Qt5Core)

set_target_properties(QDateTime PROPERTIES PREFIX "")

target_link_libraries(QDateTime Qt5::Core lua)
