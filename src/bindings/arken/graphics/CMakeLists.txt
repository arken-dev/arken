# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/arken/graphics)

if(ARKEN_GRAPHICS_IMAGE)
add_subdirectory(image)
endif()

if(ARKEN_GRAPHICS_CHART)
add_subdirectory(chart)
endif()
