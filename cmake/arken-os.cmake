# -----------------------------------------------------------------------------
# SYSTEM
# -----------------------------------------------------------------------------

set(os-system ${PROJECT_SOURCE_DIR}/src/arken/os/${CMAKE_SYSTEM_NAME}.cpp)

# -----------------------------------------------------------------------------
# UUID
# -----------------------------------------------------------------------------

set(ARKEN_OS_UUID uuidv7-xoshiro256-reference CACHE STRING "arken os uuid backend")

set_property(
  CACHE ARKEN_OS_UUID PROPERTY STRINGS libuuid libssl Linux uuidv7-basic
  uuidv7-xoshiro256-reference uuidv7-xoshiro256-superfast
)

set(os-uuid ${PROJECT_SOURCE_DIR}/src/arken/os/uuid/${ARKEN_OS_UUID}.cpp)

message("ARKEN_OS_UUID...............: ${ARKEN_OS_UUID} (libuuid|libssl|Linux)")
