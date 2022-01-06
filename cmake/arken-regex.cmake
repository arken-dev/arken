set(ARKEN_REGEX std CACHE STRING "arken regex backend")

set_property(
  CACHE ARKEN_REGEX PROPERTY STRINGS std
)

set( regex ${PROJECT_SOURCE_DIR}/src/arken/regex/${ARKEN_REGEX}.cpp )

message("ARKEN_REGEX.................: ${ARKEN_REGEX}")
