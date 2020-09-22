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
  arken_notify SHARED notify.cpp
)

find_package(Qt5Core)
find_package(Qt5Network)

set_target_properties(arken_notify PROPERTIES PREFIX "" OUTPUT_NAME "notify")

target_link_libraries(arken_notify Qt5::Core Qt5::Network arken lua)
target_link_libraries(arken_notify ${GLIB_LIBRARIES})
target_link_libraries(arken_notify ${NOTIFY_LIBRARIES})

