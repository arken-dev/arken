# ------------------------------------------------------------------------------
#
# Copyright 2016 The Charon Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/charon)

add_subdirectory(base)
add_subdirectory(base64)
add_subdirectory(cache)
add_subdirectory(digest)
add_subdirectory(doc)
add_subdirectory(http)
add_subdirectory(log)
add_subdirectory(mvm)
add_subdirectory(net)
add_subdirectory(odebug)
add_subdirectory(regex)
add_subdirectory(service)
add_subdirectory(task)
add_subdirectory(time)
add_subdirectory(utf8)
