# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/arken/net/HttpServer)

add_library(
  arken.net.HttpServer.default SHARED httpserver.cpp
  ${httpserver}
  ${PROJECT_SOURCE_DIR}/src/arken/net/httpserver/httpserver.cpp
)

set_target_properties(arken.net.HttpServer.default PROPERTIES PREFIX "" OUTPUT_NAME default)
target_link_libraries(arken.net.HttpServer.default arken)
