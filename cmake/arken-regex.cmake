set(ARKEN_REGEX "" CACHE STRING
  "arken regex backend")

set_property(
  CACHE ARKEN_REGEX PROPERTY STRINGS
  "" qt
)

if(ARKEN_REGEX STREQUAL "")
  if(ARKEN_BACKEND STREQUAL "embedded")
    message("ARKEN REGEX not contain embedded option, force qt")
    set(ARKEN_REGEX "qt")
  elseif(ARKEN_BACKEND STREQUAL "benchmark")
    set(ARKEN_REGEX "qt")
  else()
    set(ARKEN_REGEX "qt")
  endif()
endif()

# if(ARKEN_REGEX STREQUAL "embedded")
#  file(GLOB digest-embedded ${PROJECT_SOURCE_DIR}/src/embedded/utf8/*.c)
# endif()

set( regex ${PROJECT_SOURCE_DIR}/src/arken/regex/${ARKEN_REGEX}.cpp )

message("ARKEN REGEX.............: ${ARKEN_REGEX}")
