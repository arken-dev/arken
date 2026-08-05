# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

add_library(arken.graphics.ChartBar  SHARED bar.cpp)
add_library(arken.graphics.ChartLine SHARED line.cpp)
add_library(arken.graphics.ChartPie  SHARED pie.cpp)
add_library(arken.graphics.ChartArea SHARED area.cpp)

set_target_properties(arken.graphics.ChartBar  PROPERTIES PREFIX "" OUTPUT_NAME ChartBar)
set_target_properties(arken.graphics.ChartLine PROPERTIES PREFIX "" OUTPUT_NAME ChartLine)
set_target_properties(arken.graphics.ChartPie  PROPERTIES PREFIX "" OUTPUT_NAME ChartPie)
set_target_properties(arken.graphics.ChartArea PROPERTIES PREFIX "" OUTPUT_NAME ChartArea)

target_link_libraries(arken.graphics.ChartBar  arken)
target_link_libraries(arken.graphics.ChartLine arken)
target_link_libraries(arken.graphics.ChartPie  arken)
target_link_libraries(arken.graphics.ChartArea arken)
