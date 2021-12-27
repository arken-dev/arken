include_directories( /usr/pkg/include )
link_directories( /usr/pkg/lib )

if ( ARKEN_IMAGE )
  include_directories( /usr/pkg/include/ImageMagick-6 )
endif( ARKEN_IMAGE )

if ( ARKEN_NET_HTTPSERVER STREQUAL "libev" )
  include_directories( /usr/pkg/include/ev )
  link_directories( /usr/pkg/lib/ev )
endif()

if(ARKEN_NOTIFY STREQUAL "opengl")
  include_directories( /usr/X11R7/include )
  link_directories( /usr/X11R7/lib )
endif()

if(ARKEN_NOTIFY STREQUAL "glib")
  include_directories( /usr/pkg/include/glib-2.0 )
  include_directories(/usr/pkg/lib/glib-2.0/include )
endif()

if(ARKEN_NOTIFY STREQUAL "qt")
  include_directories( /usr/pkg/qt5/include/QtWidgets )
  include_directories( /usr/pkg/qt5/include )
  include_directories( /usr/pkg/qt5/include/QtCore )
  include_directories( /usr/pkg/qt5/include/QtGui )
  link_directories( /usr/pkg/qt5/lib )
endif()
