set(CHARON_BACKEND "qt" CACHE STRING
  "default backend option (qt|embedded|benchmark)")

set_property(
  CACHE CHARON_BACKEND PROPERTY STRINGS
  qt embedded benchmark
)

message("CHARON BACKEND ${CHARON_BACKEND}")
