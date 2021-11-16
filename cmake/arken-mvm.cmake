set(ARKEN_MVM_CONTAINER "" CACHE STRING
  "container implementation used by mvm")

set_property(
  CACHE ARKEN_MVM_CONTAINER PROPERTY STRINGS
  "" queue deque-stack deque-queue list-stack list-queue vector boost-lockfree-queue
)

if(ARKEN_MVM_CONTAINER STREQUAL "")
  if(ARKEN_BACKEND STREQUAL "embedded")
    set(ARKEN_MVM_CONTAINER "deque-stack")
  elseif(ARKEN_BACKEND STREQUAL "benchmark")
    set(ARKEN_MVM_CONTAINER "deque-stack")
  else()
    set(ARKEN_MVM_CONTAINER "deque-stack")
  endif()
endif()

set( mvm-container ${PROJECT_SOURCE_DIR}/src/arken/mvm/containers/${ARKEN_MVM_CONTAINER}.cpp )

message("ARKEN MVM CONTAINER.....: ${ARKEN_MVM_CONTAINER}")
