# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------
set(LWS_PATH "" CACHE STRING "alternativa path for LIBWEBSOCKETS")

if(LWS_PATH STREQUAL "")
  set(LWS_LIB "websockets")
else()
  set(LWS_LIB "${LWS_PATH}/lib/libwebsockets.so")
  set(LWS_INCLUDE "${LWS_PATH}/include")
  include_directories(${LWS_INCLUDE})
  message("LWS INCLUDE..................: ${LWS_INCLUDE}")
endif()

message("LWS LIB......................: ${LWS_LIB}")
