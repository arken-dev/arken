option( ARKEN_BARCODE "enable arken.barcode" false )

if( ARKEN_BARCODE )

  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/barcode/*.cpp)
  set( arken-barcode ${files} )

  include_directories(
    /opt/objectdata/zxing-cpp/core/src
    /opt/objectdata/zxing-cpp/thirdparty/stb
  )

  link_directories( /opt/objectdata/zxing-cpp/build/core/ )

endif()

message("ARKEN_BARCODE...............: ${ARKEN_BARCODE}")
