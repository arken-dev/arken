set(ARKEN_MVM_CONTAINER deque-stack CACHE STRING
  "container implementation used by mvm")

set_property(
  CACHE ARKEN_MVM_CONTAINER PROPERTY STRINGS
  deque-stack deque-queue
  list-stack  list-queue
  boost-lockfree-queue
  queue vector
)

set( mvm-container ${PROJECT_SOURCE_DIR}/src/arken/mvm/container/${ARKEN_MVM_CONTAINER}.cpp )

message("ARKEN_MVM_CONTAINER.........: ${ARKEN_MVM_CONTAINER} (deque-stack|deque-queue and others)")
