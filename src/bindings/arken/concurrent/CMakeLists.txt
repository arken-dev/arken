# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/arken/concurrent)

add_subdirectory(channel)
add_subdirectory(lock)
add_subdirectory(naiad)
add_subdirectory(service)
add_subdirectory(task)
add_subdirectory(task/priority)
add_subdirectory(task/balanced)
add_subdirectory(triton)
add_subdirectory(shared)
