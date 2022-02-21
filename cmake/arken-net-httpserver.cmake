set(ARKEN_NET_HTTPSERVER libev CACHE STRING "httpserver backend")

set_property(
  CACHE ARKEN_NET_HTTPSERVER PROPERTY STRINGS
  "qt/model-1"
  "boost/model-2"
  "boost/model-3"
  "linux/model-1"
  "libev"
  "libevent"
)

if(ARKEN_NOTIFY STREQUAL "qt")
  set(CMAKE_AUTOMOC ON)
  find_package(Qt5Core)
  find_package(Qt5Network)
  find_package(Qt5Gui)
  find_package(Qt5Widgets)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})
endif()

include_directories("src/arken/net/httpserver/${ARKEN_NET_HTTPSERVER}")
file(GLOB httpserver "src/arken/net/httpserver/${ARKEN_NET_HTTPSERVER}/*.cpp")

message("ARKEN_NET_HTTPSERVER........: ${ARKEN_NET_HTTPSERVER}")
