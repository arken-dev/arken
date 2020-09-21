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
  zip-binding SHARED zip.cpp
)

find_package(Qt5Core)
find_package(Qt5Network)

set_target_properties(zip-binding PROPERTIES PREFIX "" OUTPUT_NAME zip)

target_link_libraries(zip-binding arken lua)
