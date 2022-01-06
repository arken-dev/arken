# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/luasql)
include_directories(${PROJECT_SOURCE_DIR}/src/lua/luasql)

if ( LUA_MYSQL )
  add_subdirectory( mysql )
endif( LUA_MYSQL )

if( LUA_POSTGRES )
  add_subdirectory( postgres )
endif( LUA_POSTGRES )

add_subdirectory( sqlite3 )
