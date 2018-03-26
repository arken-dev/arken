# ------------------------------------------------------------------------------
#
# Copyright 2016 The Charon Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0)

option(WITHOUT_QT "with(out) QT library")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2 -finline-functions")

set(MVM_CONTAINER "default" CACHE STRING
  "container implementation used by mvm")

set_property(
  CACHE MVM_CONTAINER PROPERTY STRINGS
  default array deque_stack deque_queue list_stack list_queue vector QStack QQueue
)

if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")
endif (NOT WIN32)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

link_directories( ${PROJECT_SOURCE_DIR}/deps )

set(CMAKE_AUTOMOC ON)

if (WIN32)
find_package(curl)
find_package(lua)
endif (WIN32)

add_subdirectory( src/charon )
add_subdirectory( src/projects)
add_subdirectory( src/bindings )
add_subdirectory( src/lua )
