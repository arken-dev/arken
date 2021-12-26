option( GOOGLE_BREAKPAD "enable google breakpad support" false )

if ( GOOGLE_BREAKPAD )
  include_directories(${PROJECT_SOURCE_DIR}/src/projects/arken/include/google)
  message(${PROJECT_SOURCE_DIR}/src/projects/arken/include/google)
  include_directories( /opt/objectdata/google-breakpad/include/breakpad )
  link_directories( /opt/objectdata/google-breakpad/lib/ )
else()
  include_directories(${PROJECT_SOURCE_DIR}/src/projects/arken/include/empty)
endif()

message("GOOGLE BREAKPAD.........: ${GOOGLE_BREAKPAD}")
