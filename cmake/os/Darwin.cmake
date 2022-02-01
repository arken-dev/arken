include_directories( /opt/local/include )
link_directories( /opt/local/lib )

if( LUA_POSTGRES )
  include_directories( /opt/local/include/postgresql13 )
  link_directories( /opt/local/lib/postgresql13 )
endif()

if( LUA_MYSQL )
  include_directories( /opt/local/include/mysql57 )
  link_directories( /opt/local/lib/mysql57/mysql )
endif()
