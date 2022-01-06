set(ARKEN_BASE64 embedded CACHE STRING "arken base64 backend")

set_property(
  CACHE ARKEN_BASE64 PROPERTY STRINGS embedded libssl
)

if(ARKEN_BASE64 STREQUAL "embedded")
  file(GLOB base64-embedded ${PROJECT_SOURCE_DIR}/src/embedded/base64/*.c)
endif()

set( base64 ${PROJECT_SOURCE_DIR}/src/arken/base64/${ARKEN_BASE64}.cpp )

message("ARKEN_BASE64................: ${ARKEN_BASE64} (embedded|libssl)")
