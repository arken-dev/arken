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

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_USE_QTGUI TRUE)
set(QT_USE_QTXML TRUE)

add_executable(
  callisto
  main.cpp
  dialog.cpp
  callisto.cpp
  ../charon/charon.cpp
  dialog.h
  callisto.h
  rescource.qrc
  dialog.ui
)

#QT5_ADD_RESOURCES(rescource.qrc)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)

target_link_libraries(callisto lua charon curl)
target_link_libraries(callisto Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets)
