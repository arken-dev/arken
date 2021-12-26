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

if ( MYSQL )
  add_subdirectory( mysql )
endif( MYSQL )

if( POSTGRES )
  add_subdirectory( postgres )
endif( POSTGRES )

add_subdirectory( sqlite3 )
