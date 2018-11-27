set(CHARON_REGEX "" CACHE STRING
  "charon regex backend")

set_property(
  CACHE CHARON_REGEX PROPERTY STRINGS
  "" qt
)

if(CHARON_REGEX STREQUAL "")
  if(CHARON_BACKEND STREQUAL "embedded")
    message("CHARON REGEX not contain embedded option, force qt")
    set(CHARON_REGEX "qt")
  elseif(CHARON_BACKEND STREQUAL "benchmark")
    set(CHARON_REGEX "qt")
  else()
    set(CHARON_REGEX "qt")
  endif()
endif()

# if(CHARON_REGEX STREQUAL "embedded")
#  file(GLOB digest-embedded ${PROJECT_SOURCE_DIR}/src/embedded/utf8/*.c)
# endif()

set( regex ${PROJECT_SOURCE_DIR}/src/charon/regex/${CHARON_REGEX}.cpp )

message("CHARON REGEX ${CHARON_REGEX}")
