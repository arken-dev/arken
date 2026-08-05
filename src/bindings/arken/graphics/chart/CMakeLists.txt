# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

add_library(arken.graphics.chart.Bar  SHARED bar.cpp)
add_library(arken.graphics.chart.Line SHARED line.cpp)
add_library(arken.graphics.chart.Pie  SHARED pie.cpp)
add_library(arken.graphics.chart.Area SHARED area.cpp)

set_target_properties(arken.graphics.chart.Bar  PROPERTIES PREFIX "" OUTPUT_NAME Bar)
set_target_properties(arken.graphics.chart.Line PROPERTIES PREFIX "" OUTPUT_NAME Line)
set_target_properties(arken.graphics.chart.Pie  PROPERTIES PREFIX "" OUTPUT_NAME Pie)
set_target_properties(arken.graphics.chart.Area PROPERTIES PREFIX "" OUTPUT_NAME Area)

target_link_libraries(arken.graphics.chart.Bar  arken)
target_link_libraries(arken.graphics.chart.Line arken)
target_link_libraries(arken.graphics.chart.Pie  arken)
target_link_libraries(arken.graphics.chart.Area arken)
