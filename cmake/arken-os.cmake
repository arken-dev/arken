# -----------------------------------------------------------------------------
# NAME
# -----------------------------------------------------------------------------

set(os-name ${PROJECT_SOURCE_DIR}/src/arken/os/name/${CMAKE_SYSTEM_NAME}.cpp)

# -----------------------------------------------------------------------------
# FILE SYSTEM
# -----------------------------------------------------------------------------

set(ARKEN_OS_FILESYSTEM "" CACHE STRING
  "arken os file system backend")

set_property(
  CACHE ARKEN_OS_FILESYSTEM PROPERTY STRINGS
  "" qt boost std
)

if(ARKEN_OS_FILESYSTEM STREQUAL "")
  set(ARKEN_OS_FILESYSTEM "std")
endif()

if(ARKEN_OS_FILESYSTEM STREQUAL "boost")
  message("ARKEN OS FILESYSTEM.....: boost")
  find_package( Boost REQUIRED COMPONENTS system filesystem thread )
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(os-filesystem ${PROJECT_SOURCE_DIR}/src/arken/os/filesystem/${ARKEN_OS_FILESYSTEM}.cpp)

message("ARKEN OS FILESYSTEM.....: ${ARKEN_OS_FILESYSTEM}")

# -----------------------------------------------------------------------------
# EXECUTABLE PATH
# -----------------------------------------------------------------------------

set( os-executablePath ${PROJECT_SOURCE_DIR}/src/arken/os/executablePath/${CMAKE_SYSTEM_NAME}.cpp )

# -----------------------------------------------------------------------------
# PID
# -----------------------------------------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
set( os-pid ${PROJECT_SOURCE_DIR}/src/arken/os/pid/Windows.cpp )
else()
set( os-pid ${PROJECT_SOURCE_DIR}/src/arken/os/pid/Posix.cpp )
endif()

# -----------------------------------------------------------------------------
# UUID
# -----------------------------------------------------------------------------

set(ARKEN_OS_UUID "" CACHE STRING
  "arken os uuid backend")

set_property(
  CACHE ARKEN_OS_UUID PROPERTY STRINGS
  "" qt libuuid
)

if(ARKEN_OS_UUID STREQUAL "")
  set(ARKEN_OS_UUID "libuuid")
endif()

set(os-uuid ${PROJECT_SOURCE_DIR}/src/arken/os/uuid/${ARKEN_OS_UUID}.cpp)

message("ARKEN OS UUID...........: ${ARKEN_OS_UUID}")

if(ARKEN_OS_UUID STREQUAL "libuuid")
  message("ARKEN OS UUID not contain embedded source require install uuid-dev")
endif()
