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

add_executable( arken-bin main.cpp ${breakpad} )
set_target_properties(arken-bin PROPERTIES OUTPUT_NAME arken)

target_link_libraries(arken-bin arken)

if(GOOGLE_BREAKPAD)
  target_link_libraries(arken-bin breakpad )
  target_link_libraries(arken-bin breakpad_client )
endif()
