set(ARKEN_NOTIFY "" CACHE STRING
  "notify implementation")

set_property(
  CACHE ARKEN_NOTIFY PROPERTY STRINGS
  "" qt libnotify glib opengl
)

if(ARKEN_NOTIFY STREQUAL "")
  set(ARKEN_NOTIFY "libnotify")
endif()

if(ARKEN_NOTIFY STREQUAL "qt")
  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/notify/qt/*.cpp)
  set( arken-notify ${files} )
elseif(ARKEN_NOTIFY STREQUAL "opengl")
  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/notify/opengl/*.cpp)
  set( arken-notify ${files} )
else()
  set( arken-notify ${PROJECT_SOURCE_DIR}/src/arken/notify/${ARKEN_NOTIFY}.cpp )
endif()

message("ARKEN NOTIFY.............: ${ARKEN_NOTIFY}")
