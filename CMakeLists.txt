# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0)
project("arken")

# ------------------------------------------------------------------------------
# ARKEN CMAKE MODULES
# ------------------------------------------------------------------------------

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(google-breakpad)
include(arken)
include(arken-barcode)
include(arken-base64)
include(arken-concurrent-worker)
include(arken-digest)
include(arken-image)
include(arken-mvm)
include(arken-net-httpserver)
include(arken-os)
include(arken-regex)
include(arken-notify)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)
include_directories(${PROJECT_SOURCE_DIR}/src/embedded)
include_directories(${PROJECT_SOURCE_DIR}/src/vendors)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

if ( NOT ARKEN_DEBUG )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -finline-functions")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
endif()

if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")
endif (NOT WIN32)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

link_directories( ${PROJECT_SOURCE_DIR}/deps )

if (WIN32)
find_package(curl)
find_package(lua)
endif (WIN32)

add_subdirectory( src/arken )
add_subdirectory( src/projects )
add_subdirectory( src/bindings )
add_subdirectory( src/lua )
add_subdirectory( packages )
