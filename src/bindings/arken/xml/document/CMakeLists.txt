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
  arken.xml.Document SHARED document.cpp
)

set_target_properties(arken.xml.Document PROPERTIES PREFIX "" OUTPUT_NAME Document)
target_link_libraries(arken.xml.Document arken)
