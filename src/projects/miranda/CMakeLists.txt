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
include_directories(${PROJECT_SOURCE_DIR}/src/lua/json)

add_executable(
    miranda
    main.cpp
    mirandaserver.cpp
    mirandatask.cpp
    mirandaserver.h
    mirandatask.h
)

find_package(Qt5Core)
find_package(Qt5Network)

target_link_libraries( miranda Qt5::Core lua charon curl json )
target_link_libraries( miranda Qt5::Network )
