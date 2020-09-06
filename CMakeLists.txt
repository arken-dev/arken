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

# ------------------------------------------------------------------------------
# CHARON CMAKE MODULES
# ------------------------------------------------------------------------------

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(charon)
include(charon-backend)
include(charon-base64)
include(charon-digest)
include(charon-mvm)
include(charon-os)
include(charon-regex)
include(charon-utf8)
include(arken-notify)

option(WITHOUT_QT "with(out) QT library")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)
include_directories(${PROJECT_SOURCE_DIR}/src/embedded)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2 -finline-functions")

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
add_subdirectory( src/projects )
add_subdirectory( src/bindings )
add_subdirectory( src/lua )
add_subdirectory( packages )
