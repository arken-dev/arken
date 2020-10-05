<% local mvm = require('arken.mvm') -%>
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

# ------------------------------------------------------------------------------
# ARKEN CMAKE MODULES
# ------------------------------------------------------------------------------

# LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# include(mymodule)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(<%= mvm:path() -%>/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2 -finline-functions")

if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")
endif (NOT WIN32)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

link_directories( <%= mvm:path() -%>/deps )

file(GLOB src ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable( myproject ${src} )

target_link_libraries(myproject lua arken curl)

if (WIN32)
find_package(curl)
find_package(lua)
endif (WIN32)
