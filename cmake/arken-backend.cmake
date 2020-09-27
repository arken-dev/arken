set(ARKEN_BACKEND "qt" CACHE STRING
  "default backend option (qt|embedded|benchmark)")

set_property(
  CACHE ARKEN_BACKEND PROPERTY STRINGS
  qt embedded benchmark
)

message("ARKEN BACKEND...........: ${ARKEN_BACKEND}")
