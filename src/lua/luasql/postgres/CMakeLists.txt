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
set(LUA_POSTGRES_LIB pq CACHE STRING "alternativa path for libpq.so")

add_library(
  postgres SHARED
  ls_postgres.c
  ../luasql.c
)

set_target_properties(postgres PROPERTIES PREFIX "")

target_link_libraries(postgres arken ${LUA_POSTGRES_LIB})
