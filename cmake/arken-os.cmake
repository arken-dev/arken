# -----------------------------------------------------------------------------
# NAME
# -----------------------------------------------------------------------------

set(os-name ${PROJECT_SOURCE_DIR}/src/charon/os/name/${CMAKE_SYSTEM_NAME}.cpp)

# -----------------------------------------------------------------------------
# FILE SYSTEM
# -----------------------------------------------------------------------------

set(CHARON_OS_FILESYSTEM "" CACHE STRING
  "charon os file system backend")

set_property(
  CACHE CHARON_OS_FILESYSTEM PROPERTY STRINGS
  "" qt boost
)

if(CHARON_OS_FILESYSTEM STREQUAL "")
  set(CHARON_OS_FILESYSTEM "qt")
endif()

if(CHARON_OS_FILESYSTEM STREQUAL "boost")
  message("CHARON OS FILESYSTEM.....: boost not not available, force qt")
  set(CHARON_OS_FILESYSTEM "qt")
endif()

set(os-filesystem ${PROJECT_SOURCE_DIR}/src/charon/os/filesystem/${CHARON_OS_FILESYSTEM}.cpp)

message("CHARON OS FILESYSTEM.....: ${CHARON_OS_FILESYSTEM}")

# -----------------------------------------------------------------------------
# EXECUTABLE PATH
# -----------------------------------------------------------------------------

set( os-executablePath ${PROJECT_SOURCE_DIR}/src/charon/os/executablePath/${CMAKE_SYSTEM_NAME}.cpp )

# -----------------------------------------------------------------------------
# PID
# -----------------------------------------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
set( os-pid ${PROJECT_SOURCE_DIR}/src/charon/os/pid/Windows.cpp )
else()
set( os-pid ${PROJECT_SOURCE_DIR}/src/charon/os/pid/Posix.cpp )
endif()

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

set(os-uuid ${PROJECT_SOURCE_DIR}/src/charon/os/uuid/${CHARON_OS_UUID}.cpp)

message("CHARON OS UUID...........: ${CHARON_OS_UUID}")

if(CHARON_OS_UUID STREQUAL "libuuid")
  message("CHARON OS UUID not contain embedded source require install uuid-dev")
endif()
