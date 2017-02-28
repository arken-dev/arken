# ------------------------------------------------------------------------------
#
# Copyright 2016 The Charon Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/luasql)
include_directories(${PROJECT_SOURCE_DIR}/src/lua/luasql)

if ( NOT WIN32 )
  add_subdirectory( mysql )
  add_subdirectory( postgres )
endif( NOT WIN32 )

add_subdirectory( sqlite3 )
