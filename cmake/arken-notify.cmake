set(ARKEN_NOTIFY "" CACHE STRING
  "notify implementation")

set_property(
  CACHE ARKEN_NOTIFY PROPERTY STRINGS
  "" qt libnotify glib
)

if(ARKEN_NOTIFY STREQUAL "")
  set(ARKEN_NOTIFY "libnotify")
endif()

set( arken-notify ${PROJECT_SOURCE_DIR}/src/charon/notify/${ARKEN_NOTIFY}.cpp )

message("ARKEN NOTIFY.............: ${ARKEN_NOTIFY}")
