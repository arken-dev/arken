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

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/luasql)

add_library(
  mysql SHARED
  ls_mysql.c
  ../luasql.c
)

set_target_properties(mysql PROPERTIES PREFIX "")

target_link_libraries(mysql arken mysqlclient)
