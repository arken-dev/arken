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
include_directories(${PROJECT_SOURCE_DIR}/src/lua/json)

link_directories( ${PROJECT_SOURCE_DIR}/deps )

set(SRCS
    Channel.cpp
    Epoll.cpp
    EventLoop.cpp
    EventLoopThread.cpp
    EventLoopThreadPool.cpp
    HttpData.cpp
    Main.cpp
    Server.cpp
    #ThreadPool.cpp
    Timer.cpp
    Util.cpp
)

find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Gui)
find_package(Qt5Widgets)


#set(EXECUTABLE_OUTPUT_PATH /home/ariveira/projetos/arken/bin)

add_executable(titan ${SRCS})

target_link_libraries(titan libserver_base arken lua)
target_link_libraries(titan Qt5::Core)
target_link_libraries(titan Qt5::Network)

add_subdirectory(base)
