# ------------------------------------------------------------------------------
#
# Copyright 2016 The Arken Platform Authors.
#
# All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
#
# ------------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/clib/arken/doc)

add_library(
  arken.doc.Pdf SHARED pdf.cpp
)

set_target_properties(arken.doc.Pdf PROPERTIES PREFIX "" OUTPUT_NAME Pdf)
target_link_libraries(arken.doc.Pdf arken)
