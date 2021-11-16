set(ARKEN_BASE64 "" CACHE STRING
  "arken base64 backend")

set_property(
  CACHE ARKEN_BASE64 PROPERTY STRINGS
  "" libssl embedded
)

if(ARKEN_BASE64 STREQUAL "")
  if(ARKEN_BACKEND STREQUAL "embedded")
    set(ARKEN_BASE64 "embedded")
  elseif(ARKEN_BACKEND STREQUAL "benchmark")
    set(ARKEN_BASE64 "embedded")
  else()
    set(ARKEN_BASE64 "embedded")
  endif()
endif()

if(ARKEN_BASE64 STREQUAL "embedded")
  file(GLOB base64-embedded ${PROJECT_SOURCE_DIR}/src/embedded/base64/*.c)
endif()

set( base64 ${PROJECT_SOURCE_DIR}/src/arken/base64/${ARKEN_BASE64}.cpp )

message("ARKEN BASE64............: ${ARKEN_BASE64}")
