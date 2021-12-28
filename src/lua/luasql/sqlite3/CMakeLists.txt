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
include_directories(${PROJECT_SOURCE_DIR}/src/vendors/sqlite3)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/luasql)

add_library(
  sqlite3 SHARED
  ls_sqlite3.c
  ../luasql.c
  ../../../vendors/sqlite3/sqlite3.c
)

set_target_properties(sqlite3 PROPERTIES PREFIX "")

target_link_libraries(sqlite3 arken)
