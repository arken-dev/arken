# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

add_library(
  arken.net.WebSocketConnection SHARED websocketconnection.cpp
)

set_target_properties(arken.net.WebSocketConnection PROPERTIES PREFIX "" OUTPUT_NAME WebSocketConnection)
target_link_libraries(arken.net.WebSocketConnection arken)
