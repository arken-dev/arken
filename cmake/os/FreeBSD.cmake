include_directories( /usr/local/include )
link_directories( /usr/local/lib )

if( LUA_MYSQL )
  link_directories( /usr/local/lib/mysql )
endif()

if(ARKEN_NOTIFY STREQUAL "glib")
  include_directories( /usr/local/include/glib-2.0 )
  include_directories( /usr/local/lib/glib-2.0/include )
endif()
