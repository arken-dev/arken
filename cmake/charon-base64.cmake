set(CHARON_BASE64 "" CACHE STRING
  "charon base64 backend")

set_property(
  CACHE CHARON_BASE64 PROPERTY STRINGS
  "" qt embedded
)

if(CHARON_BASE64 STREQUAL "")
  if(CHARON_BACKEND STREQUAL "embedded")
    set(CHARON_BASE64 "embedded")
  elseif(CHARON_BACKEND STREQUAL "benchmark")
    set(CHARON_BASE64 "embedded")
  else()
    set(CHARON_BASE64 "qt")
  endif()
endif()

if(CHARON_BASE64 STREQUAL "embedded")
  file(GLOB base64-embedded ${PROJECT_SOURCE_DIR}/src/embedded/base64/*.c)
endif()

set( base64 ${PROJECT_SOURCE_DIR}/src/charon/base64/${CHARON_BASE64}.cpp )

message("CHARON BASE64 ${CHARON_BASE64}")
