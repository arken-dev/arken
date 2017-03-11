# ------------------------------------------------------------------------------
#
# Copyright 2016 The Charon Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)
include_directories(${PROJECT_SOURCE_DIR}/src/vendors/ap)
include_directories(${PROJECT_SOURCE_DIR}/src/vendors/mongrel2)
include_directories(${PROJECT_SOURCE_DIR}/src/vendors/mongrel2/http11)

file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB net ${CMAKE_CURRENT_SOURCE_DIR}/net/*.cpp)
file(GLOB time ${CMAKE_CURRENT_SOURCE_DIR}/time/*.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deps)
set(mongrel2 ${PROJECT_SOURCE_DIR}/src/vendors/mongrel2/http11/http11_parser.c)
set(ap ${PROJECT_SOURCE_DIR}/src/vendors/ap/ap_base64.c)

add_library( charon SHARED ${src} ${net} ${time} ${mongrel2} ${ap} )

find_package(Qt5Core)
find_package(Qt5Network)

target_link_libraries(charon Qt5::Core)
target_link_libraries(charon Qt5::Network)
target_link_libraries(charon curl)
target_link_libraries(charon lua)
