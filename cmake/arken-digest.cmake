set(ARKEN_DIGEST "" CACHE STRING
  "arken digest (md5|sha1) backend")

set_property(
  CACHE ARKEN_DIGEST PROPERTY STRINGS
  "" embedded libssl
)

if(ARKEN_DIGEST STREQUAL "")
  set(ARKEN_DIGEST "embedded")
endif()

if(ARKEN_DIGEST STREQUAL "embedded")
  file(GLOB digest-embedded ${PROJECT_SOURCE_DIR}/src/embedded/digest/*.c)
endif()

file(GLOB digest ${PROJECT_SOURCE_DIR}/src/arken/digest/*/${ARKEN_DIGEST}.cpp)
#file(GLOB_RECURSE digest ${PROJECT_SOURCE_DIR}/src/arken/digest/*/${ARKEN_DIGEST}.cpp)

message("ARKEN DIGEST............: ${ARKEN_DIGEST}")
