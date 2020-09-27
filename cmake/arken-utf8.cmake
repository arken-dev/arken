set(ARKEN_UTF8 "" CACHE STRING
  "arken uf8 backend")

set_property(
  CACHE ARKEN_UTF8 PROPERTY STRINGS
  "" qt
)

if(ARKEN_UTF8 STREQUAL "")
  if(ARKEN_BACKEND STREQUAL "embedded")
    message("ARKEN UTF8 not contain embedded option, force qt")
    set(ARKEN_UTF8 "qt")
  elseif(ARKEN_BACKEND STREQUAL "benchmark")
    set(ARKEN_UTF8 "qt")
  else()
    set(ARKEN_UTF8 "qt")
  endif()
endif()

# if(ARKEN_UTF8 STREQUAL "embedded")
#  file(GLOB digest-embedded ${PROJECT_SOURCE_DIR}/src/embedded/utf8/*.c)
# endif()

set( utf8 ${PROJECT_SOURCE_DIR}/src/arken/utf8/${ARKEN_UTF8}.cpp )

message("ARKEN UTF8..............: ${ARKEN_UTF8}")
