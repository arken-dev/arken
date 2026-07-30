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
  arken.crypto.aes SHARED aes.cpp
)

set_target_properties(arken.crypto.aes PROPERTIES PREFIX "" OUTPUT_NAME aes)
target_link_libraries(arken.crypto.aes arken)
