# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
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

file(GLOB src      ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB os       ${CMAKE_CURRENT_SOURCE_DIR}/os/*.c)
file(GLOB os-glob  ${PROJECT_SOURCE_DIR}/src/vendors/apple/glob.c)
file(GLOB net      ${CMAKE_CURRENT_SOURCE_DIR}/net/*.cpp)
file(GLOB concurrent ${CMAKE_CURRENT_SOURCE_DIR}/concurrent/*.cpp)
file(GLOB concurrent-task ${CMAKE_CURRENT_SOURCE_DIR}/concurrent/task/*.cpp)
file(GLOB compress ${CMAKE_CURRENT_SOURCE_DIR}/compress/*.cpp)
file(GLOB time     ${CMAKE_CURRENT_SOURCE_DIR}/time/*.cpp)
file(GLOB chrono   ${CMAKE_CURRENT_SOURCE_DIR}/chrono/*.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deps)
set(mongrel2 ${PROJECT_SOURCE_DIR}/src/vendors/mongrel2/http11/http11_parser.c)

add_library(
  arken SHARED
  ${src}
  ${arken-barcode}
  ${arken-image}
  ${base64}
  ${base64-embedded}
  ${compress}
  ${chrono}
  ${digest}
  ${digest-embedded}
  ${mvm-container}
  ${net}
  ${httpserver}
  ${arken-notify}
  ${os} ${os-system} ${os-uuid} ${os-glob}
  ${concurrent} ${concurrent-task} ${arken-concurrent-worker}
  ${regex}
  ${time}
  ${mongrel2}
  ${utf8}
)

target_link_libraries(arken pthread)
target_link_libraries(arken m)
target_link_libraries(arken zip)
target_link_libraries(arken curl)
target_link_libraries(arken lua)
target_link_libraries(arken json)
target_link_libraries(arken ${ImageMagick_LIBRARIES})

#------------------------------------------------------------------------------
# ARKEN_IMAGE
#------------------------------------------------------------------------------

if(ARKEN_IMAGE)
  target_link_libraries(arken ${ImageMagick_MagickWand_LIBRARY})
endif()

#------------------------------------------------------------------------------
# ARKEN_NOTIFY
#------------------------------------------------------------------------------

if(ARKEN_NOTIFY STREQUAL "libnotify")
  target_link_libraries(arken notify)
  target_link_libraries(arken ${GLIB_LIBRARIES})
  target_link_libraries(arken ${NOTIFY_LIBRARIES})
endif()

if(ARKEN_NOTIFY STREQUAL "qt")
  target_link_libraries(arken Qt5Core Qt5Network Qt5Gui Qt5Widgets)
  include_directories(${PROJECT_SOURCE_DIR}/src/arken/notify/qt)
endif()

if(ARKEN_NOTIFY STREQUAL "opengl")
  include_directories(${PROJECT_SOURCE_DIR}/src/arken/notify/opengl)
endif()

if(ARKEN_NOTIFY STREQUAL "glib")
  message(STATUS ${GLIB_LIBRARIES})
  target_link_libraries(arken ${GLIB_LIBRARIES})
  target_link_libraries(arken gio-2.0)
endif()

if(ARKEN_NOTIFY STREQUAL "opengl")
  target_link_libraries(arken glut GL)
endif()

#------------------------------------------------------------------------------
# ARKEN_DIGEST
#------------------------------------------------------------------------------

if(ARKEN_DIGEST STREQUAL "libssl")
  target_link_libraries(arken ssl)
endif()

#------------------------------------------------------------------------------
# ARKEN_OS_UUID
#------------------------------------------------------------------------------

if(ARKEN_OS_UUID STREQUAL "libuuid")
  target_link_libraries(arken uuid)
endif()

if(ARKEN_OS_UUID STREQUAL "libssl")
  target_link_libraries(arken ssl)
  target_link_libraries(arken crypto)
endif()

#------------------------------------------------------------------------------
# ARKEN_OS_FILESYSTEM
#------------------------------------------------------------------------------

if(ARKEN_OS_FILESYSTEM STREQUAL "boost")
  message(STATUS "link libarken with ${Boost_LIBRARIES}")
  target_link_libraries(arken pthread m ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
endif()

#------------------------------------------------------------------------------
# ARKEN_NET_HTTPSERVER
#------------------------------------------------------------------------------

if(ARKEN_NET_HTTPSERVER STREQUAL "qt")
  target_link_libraries(arken Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets)
endif()

if(ARKEN_NET_HTTPSERVER STREQUAL "linux/model-1")
  add_subdirectory(${PROJECT_SOURCE_DIR}/src/arken/net/httpserver/linux/model-1/base)
  target_link_libraries(arken libserver_base)
endif()

if(ARKEN_NET_HTTPSERVER STREQUAL "boost/model-2")
  find_package(Boost COMPONENTS system thread)
  find_package(Threads REQUIRED)
  SET(Boost_USE_MULTITHREADED ON)
  target_link_libraries(arken pthread m ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
endif()

if(ARKEN_NET_HTTPSERVER STREQUAL "boost/model-3")
  find_package(Boost COMPONENTS system thread)
  find_package(Threads REQUIRED)
  SET(Boost_USE_MULTITHREADED ON)
  target_link_libraries(arken pthread m ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
endif()

if(ARKEN_NET_HTTPSERVER STREQUAL "libev")
  target_link_libraries(arken ev)
endif()

#------------------------------------------------------------------------------
# ARKEN_BARCODE
#------------------------------------------------------------------------------

if(ARKEN_BARCODE)
  target_link_libraries( arken ZXing )
endif()
