# -----------------------------------------------------------------------------
# NAME
# -----------------------------------------------------------------------------

set(os-system ${PROJECT_SOURCE_DIR}/src/arken/os/${CMAKE_SYSTEM_NAME}.cpp)

# -----------------------------------------------------------------------------
# UUID
# -----------------------------------------------------------------------------

set(ARKEN_OS_UUID "" CACHE STRING
  "arken os uuid backend")

set_property(
  CACHE ARKEN_OS_UUID PROPERTY STRINGS
  "" libuuid libssl Linux
)

if(ARKEN_OS_UUID STREQUAL "")
  set(ARKEN_OS_UUID "libssl")
endif()

if(ARKEN_OS_UUID STREQUAL "libuuid")
  message("ARKEN OS UUID not contain embedded source require install uuid-dev")
endif()

set(os-uuid ${PROJECT_SOURCE_DIR}/src/arken/os/uuid/${ARKEN_OS_UUID}.cpp)

message("ARKEN OS UUID...........: ${ARKEN_OS_UUID}")
