option( ARKEN_BARCODE "enable arken.barcode" false )

if( ARKEN_BARCODE )

  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/barcode/*.cpp)
  set( arken-barcode ${files} )

  include_directories(
    /opt/objectdata/zxing/include/ZXing
    /opt/objectdata/zxing/include/stb
  )

  link_directories( /opt/objectdata/zxing/lib/ )

endif()

message("ARKEN_BARCODE...............: ${ARKEN_BARCODE}")
