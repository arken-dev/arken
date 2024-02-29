# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------
set(CURL_PATH "" CACHE STRING "alternativa path for LIBCURL")

if(CURL_PATH STREQUAL "")
  set(CURL_LIB "curl")
else()
  set(CURL_LIB "${CURL_PATH}/lib/libcurl.so")
  set(CURL_INCLUDE "${CURL_PATH}/include")
  include_directories(LIBCURL_INCLUDE)
  message("CURL INCLUDE................: ${CURL_INCLUDE}")
endif()

message("CURL LIB....................: ${CURL_LIB}")
