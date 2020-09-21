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

add_library(
  QSqlQuery SHARED QSqlQuery.cpp
)

find_package(Qt5Sql)

set_target_properties(QSqlQuery PROPERTIES PREFIX "")

target_link_libraries(QSqlQuery Qt5::Sql lua)
