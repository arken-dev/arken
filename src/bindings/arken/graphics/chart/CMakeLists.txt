# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

add_library(arken.graphics.chart.Bar         SHARED bar.cpp)
add_library(arken.graphics.chart.Line        SHARED line.cpp)
add_library(arken.graphics.chart.Pie         SHARED pie.cpp)
add_library(arken.graphics.chart.Area        SHARED area.cpp)
add_library(arken.graphics.chart.StackedBar  SHARED stacked_bar.cpp)
add_library(arken.graphics.chart.SideBar     SHARED side_bar.cpp)
add_library(arken.graphics.chart.StackedArea SHARED stacked_area.cpp)

set_target_properties(arken.graphics.chart.Bar         PROPERTIES PREFIX "" OUTPUT_NAME Bar)
set_target_properties(arken.graphics.chart.Line        PROPERTIES PREFIX "" OUTPUT_NAME Line)
set_target_properties(arken.graphics.chart.Pie         PROPERTIES PREFIX "" OUTPUT_NAME Pie)
set_target_properties(arken.graphics.chart.Area        PROPERTIES PREFIX "" OUTPUT_NAME Area)
set_target_properties(arken.graphics.chart.StackedBar  PROPERTIES PREFIX "" OUTPUT_NAME StackedBar)
set_target_properties(arken.graphics.chart.SideBar     PROPERTIES PREFIX "" OUTPUT_NAME SideBar)
set_target_properties(arken.graphics.chart.StackedArea PROPERTIES PREFIX "" OUTPUT_NAME StackedArea)

target_link_libraries(arken.graphics.chart.Bar         arken)
target_link_libraries(arken.graphics.chart.Line        arken)
target_link_libraries(arken.graphics.chart.Pie         arken)
target_link_libraries(arken.graphics.chart.Area        arken)
target_link_libraries(arken.graphics.chart.StackedBar  arken)
target_link_libraries(arken.graphics.chart.SideBar     arken)
target_link_libraries(arken.graphics.chart.StackedArea arken)
