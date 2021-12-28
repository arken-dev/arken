# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/arken)

add_subdirectory(base)

if(ARKEN_BARCODE)
add_subdirectory(barcode)
endif()

add_subdirectory(base64)
add_subdirectory(cache)
add_subdirectory(chrono)
add_subdirectory(compress)
add_subdirectory(digest)

if(ARKEN_IMAGE)
add_subdirectory(image)
endif()

add_subdirectory(json)
add_subdirectory(log)
add_subdirectory(mvm)
add_subdirectory(net)
add_subdirectory(odebug)
add_subdirectory(concurrent)

if(NOT ARKEN_NOTIFY STREQUAL "")
  add_subdirectory(notify)
endif()

add_subdirectory(regex)
add_subdirectory(utf8)
