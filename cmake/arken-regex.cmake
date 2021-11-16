set(ARKEN_REGEX "" CACHE STRING
  "arken regex backend")

set_property(
  CACHE ARKEN_REGEX PROPERTY STRINGS
  "" std
)

if(ARKEN_REGEX STREQUAL "")
  set(ARKEN_REGEX "std")
endif()

set( regex ${PROJECT_SOURCE_DIR}/src/arken/regex/${ARKEN_REGEX}.cpp )

message("ARKEN REGEX.............: ${ARKEN_REGEX}")
