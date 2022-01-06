set(ARKEN_CONCURRENT_WORKER "mvm" CACHE STRING "standalone mvm")

set_property(
  CACHE ARKEN_CONCURRENT_WORKER PROPERTY STRINGS standalone mvm
)

set( arken-concurrent-worker ${PROJECT_SOURCE_DIR}/src/arken/concurrent/worker/${ARKEN_CONCURRENT_WORKER}.cpp )

message("ARKEN_CONCURRENT_WORKER.....: ${ARKEN_CONCURRENT_WORKER} (standalone|mvm)")
