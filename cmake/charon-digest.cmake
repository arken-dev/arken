set(CHARON_DIGEST "" CACHE STRING
  "charon digest (md5|sha1) backend")

set_property(
  CACHE CHARON_DIGEST PROPERTY STRINGS
  "" qt embedded
)

if(CHARON_DIGEST STREQUAL "")
  if(CHARON_BACKEND STREQUAL "embedded")
    set(CHARON_DIGEST "embedded")
  elseif(CHARON_BACKEND STREQUAL "benchmark")
    set(CHARON_DIGEST "qt")
  else()
    set(CHARON_DIGEST "qt")
  endif()
endif()

if(CHARON_DIGEST STREQUAL "embedded")
  file(GLOB digest-embedded ${PROJECT_SOURCE_DIR}/src/embedded/digest/*.c)
endif()

set( digest ${PROJECT_SOURCE_DIR}/src/charon/digest/${CHARON_DIGEST}.cpp )

message("CHARON DIGEST ${CHARON_DIGEST}")
