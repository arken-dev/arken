# -----------------------------------------------------------------------------
# EXECUTABLE PATH
# -----------------------------------------------------------------------------

set( os-executablePath ${PROJECT_SOURCE_DIR}/src/charon/os/executablePath_${CMAKE_SYSTEM_NAME}.cpp )

# -----------------------------------------------------------------------------
# UUID
# -----------------------------------------------------------------------------

set(CHARON_OS_UUID "" CACHE STRING
  "charon os uuid backend")

set_property(
  CACHE CHARON_OS_UUID PROPERTY STRINGS
  "" qt libuuid
)

if(CHARON_OS_UUID STREQUAL "")
  if(CHARON_BACKEND STREQUAL "embedded")
    set(CHARON_OS_UUID "libuuid")
  elseif(CHARON_BACKEND STREQUAL "benchmark")
    set(CHARON_OS_UUID "qt")
  else()
    set(CHARON_OS_UUID "qt")
  endif()
endif()

set(os-uuid ${PROJECT_SOURCE_DIR}/src/charon/os/uuid_${CHARON_OS_UUID}.cpp)

message("CHARON OS UUID ${CHARON_OS_UUID}")

if(CHARON_OS_UUID STREQUAL "libuuid")
  message("CHARON OS UUID not contain embedded source require install uuid-dev")
endif()
