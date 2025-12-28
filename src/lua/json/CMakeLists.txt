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

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deps)

add_library(
  json SHARED
  json.h dtoa_config.h fpconv.h strbuf.h
  json.c strbuf.c g_fmt.c dtoa.c #fpconv.c
)

# target_link_libraries(json arken)
