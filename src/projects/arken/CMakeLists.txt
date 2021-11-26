# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)

if(GOOGLE_BREAKPAD)
  MESSAGE("google breakpad link arken-bin")
  set(GOOGLE_BREAKPAD_ENABLED true)
  include_directories(/opt/objectdata/google-breakpad/include/breakpad)
endif()

add_executable( arken-bin main.cpp )
set_target_properties(arken-bin PROPERTIES OUTPUT_NAME arken)

target_link_libraries(arken-bin lua arken curl)
if(GOOGLE_BREAKPAD)
  target_link_libraries(arken-bin /opt/objectdata/google-breakpad/lib/libbreakpad.a )
  target_link_libraries(arken-bin /opt/objectdata/google-breakpad/lib/libbreakpad_client.a )
endif()
