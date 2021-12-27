include_directories( /usr/local/include )
link_directories( /usr/local/lib )

if( POSTGRES )
  include_directories( /usr/local/include/postgresql )
endif( POSTGRES )

if(ARKEN_NOTIFY STREQUAL "opengl")
  include_directories( /usr/X11R6/include )
  link_directories( /usr/X11R6/lib )
endif()

if(ARKEN_NOTIFY STREQUAL "glib")
  include_directories( /usr/local/include/glib-2.0 )
  include_directories( /usr/local/lib/glib-2.0/include )
endif()
