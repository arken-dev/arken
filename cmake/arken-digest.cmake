set(ARKEN_DIGEST embedded CACHE STRING "arken digest (md5|sha1) backend")

set_property(
  CACHE ARKEN_DIGEST PROPERTY STRINGS embedded libssl
)

if(ARKEN_DIGEST STREQUAL "embedded")
  file(GLOB digest-embedded ${PROJECT_SOURCE_DIR}/src/embedded/digest/*.c)
endif()

file(GLOB digest ${PROJECT_SOURCE_DIR}/src/arken/digest/*/${ARKEN_DIGEST}.cpp)

message("ARKEN_DIGEST................: ${ARKEN_DIGEST} (embedded|libssl)")
