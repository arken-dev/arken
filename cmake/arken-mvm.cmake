set(CHARON_MVM_CONTAINER "" CACHE STRING
  "container implementation used by mvm")

set_property(
  CACHE CHARON_MVM_CONTAINER PROPERTY STRINGS
  "" deque-stack deque-queue list-stack list-queue vector QQueue boost-lockfree-queue
)

if(CHARON_MVM_CONTAINER STREQUAL "")
  if(CHARON_BACKEND STREQUAL "embedded")
    set(CHARON_MVM_CONTAINER "deque-stack")
  elseif(CHARON_BACKEND STREQUAL "benchmark")
    set(CHARON_MVM_CONTAINER "deque-stack")
  else()
    set(CHARON_MVM_CONTAINER "deque-stack")
  endif()
endif()

set( mvm-container ${PROJECT_SOURCE_DIR}/src/charon/mvm/containers/${CHARON_MVM_CONTAINER}.cpp )

message("CHARON MVM CONTAINER.....: ${CHARON_MVM_CONTAINER}")
