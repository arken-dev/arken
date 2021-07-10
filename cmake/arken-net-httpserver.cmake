set(ARKEN_NET_HTTPSERVER "" CACHE STRING
  "httpserver backend")

set_property(
  CACHE ARKEN_NET_HTTPSERVER PROPERTY STRINGS
  ""
  "qt/model-1"
  "boost/model-2"
  "boost/model-3"
  "linux/model-1"
)


if(ARKEN_NET_HTTPSERVER STREQUAL "")
  set(ARKEN_NET_HTTPSERVER "qt/model-1")
endif()

include_directories("src/arken/net/httpserver/${ARKEN_NET_HTTPSERVER}")
file(GLOB httpserver "src/arken/net/httpserver/${ARKEN_NET_HTTPSERVER}/*.cpp")


message("ARKEN NET_HTTPSERVER....: ${ARKEN_NET_HTTPSERVER}")
