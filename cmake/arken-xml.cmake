option( ARKEN_XML "enable arken.xml" false )

if( ARKEN_XML )
  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/xml/*.cpp)
  set( arken-xml ${files} )
  find_package(LibXml2 REQUIRED)
  include_directories(${LIBXML2_INCLUDE_DIR})
endif()

message("ARKEN_XML...................: ${ARKEN_XML}")
