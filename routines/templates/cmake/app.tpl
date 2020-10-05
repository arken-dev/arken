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

find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Gui)
find_package(Qt5Widgets)
set(CMAKE_AUTOMOC ON)


# ------------------------------------------------------------------------------
# ARKEN CMAKE MODULES
# ------------------------------------------------------------------------------

# LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# include(mymodule)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(<%= mvm:path() -%>/include)
include_directories(<%= mvm:path() -%>/deps/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2 -finline-functions")

if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")
endif (NOT WIN32)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

link_directories( <%= mvm:path() -%>/deps )

file(GLOB src ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable( <%= self.name -%> ${src} )

target_link_libraries(<%= self.name -%> lua arken curl)
target_link_libraries(<%= self.name -%> Qt5::Core)
target_link_libraries(<%= self.name -%> Qt5::Network)
target_link_libraries(<%= self.name -%> Qt5::Gui)
target_link_libraries(<%= self.name -%> Qt5::Widgets)

if (WIN32)
find_package(curl)
find_package(lua)
endif (WIN32)
