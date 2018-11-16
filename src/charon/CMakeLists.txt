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

file(GLOB src  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB os   ${CMAKE_CURRENT_SOURCE_DIR}/os/*.c)
file(GLOB net  ${CMAKE_CURRENT_SOURCE_DIR}/net/*.cpp)
file(GLOB doc  ${CMAKE_CURRENT_SOURCE_DIR}/doc/*.cpp)
file(GLOB time ${CMAKE_CURRENT_SOURCE_DIR}/time/*.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deps)
set(mongrel2 ${PROJECT_SOURCE_DIR}/src/vendors/mongrel2/http11/http11_parser.c)

add_library(
  charon SHARED
  ${src}
  ${base64}
  ${base64-embedded}
  ${digest}
  ${digest-embedded}
  ${mvm-container}
<<<<<<< HEAD
  ${os} ${os-executablePath} ${uuid}
  ${doc}
=======
>>>>>>> master
  ${net}
  ${os} ${os-executablePath} ${os-uuid}
  ${regex}
  ${time}
  ${mongrel2}
  ${utf8}
)

find_package(Qt5Core)
find_package(Qt5Network)

target_link_libraries(charon Qt5::Core)
target_link_libraries(charon Qt5::Network)
target_link_libraries(charon curl)
target_link_libraries(charon lua)
if(CHARON_DIGEST STREQUAL "libssl")
  target_link_libraries(charon ssl)
endif()
if(CHARON_OS_UUID STREQUAL "libuuid")
  target_link_libraries(charon uuid)
endif()
