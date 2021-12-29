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
  arken.net.HttpClient SHARED httpclient.cpp
)

set_target_properties(arken.net.HttpClient PROPERTIES PREFIX "" OUTPUT_NAME HttpClient)
target_link_libraries(arken.net.HttpClient arken)
