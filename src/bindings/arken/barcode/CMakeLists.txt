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
  arken.Barcode SHARED barcode.cpp
)

set_target_properties(arken.Barcode PROPERTIES PREFIX "" OUTPUT_NAME Barcode)
target_link_libraries(arken.Barcode arken)
