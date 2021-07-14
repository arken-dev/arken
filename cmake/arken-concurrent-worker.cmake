set(ARKEN_CONCURRENT_WORKER "" CACHE STRING
  "standalone mvm")

set_property(
  CACHE ARKEN_CONCURRENT_WORKER PROPERTY STRINGS
  "" standalone mvm
)

if(ARKEN_CONCURRENT_WORKER STREQUAL "")
  set(ARKEN_CONCURRENT_WORKER "mvm")
endif()

set( arken-concurrent-worker ${PROJECT_SOURCE_DIR}/src/arken/concurrent/worker/${ARKEN_CONCURRENT_WORKER}.cpp )

message("ARKEN CONCURRENT_WORKER.: ${ARKEN_CONCURRENT_WORKER}")
