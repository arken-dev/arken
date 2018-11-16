set(CHARON_UTF8 "" CACHE STRING
  "charon uf8 backend")

set_property(
  CACHE CHARON_UTF8 PROPERTY STRINGS
  "" qt
)

if(CHARON_UTF8 STREQUAL "")
  if(CHARON_BACKEND STREQUAL "embedded")
    message("CHARON UTF8 not contain embedded option, force qt")
    set(CHARON_UTF8 "qt")
  elseif(CHARON_BACKEND STREQUAL "benchmark")
    set(CHARON_UTF8 "qt")
  else()
    set(CHARON_UTF8 "qt")
  endif()
endif()

# if(CHARON_UTF8 STREQUAL "embedded")
#  file(GLOB digest-embedded ${PROJECT_SOURCE_DIR}/src/embedded/utf8/*.c)
# endif()

set( utf8 ${PROJECT_SOURCE_DIR}/src/charon/utf8/${CHARON_UTF8}.cpp )

message("CHARON UTF8 ${CHARON_UTF8}")
