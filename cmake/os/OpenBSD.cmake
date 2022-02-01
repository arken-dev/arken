include_directories( /usr/local/include )
link_directories( /usr/local/lib )

if( LUA_POSTGRES )
  include_directories( /usr/local/include/postgresql )
endif()

if(ARKEN_NOTIFY STREQUAL "opengl")
  include_directories( /usr/X11R6/include )
  link_directories( /usr/X11R6/lib )
endif()

if(ARKEN_NOTIFY STREQUAL "glib")
  include_directories( /usr/local/include/glib-2.0 )
  include_directories( /usr/local/lib/glib-2.0/include )
endif()
