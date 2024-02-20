# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/arken/net)

add_subdirectory(httpbody)
add_subdirectory(httpclient)
add_subdirectory(httpenv)
add_subdirectory(httpserver)
add_subdirectory(httpserver/default)
add_subdirectory(smtp)
