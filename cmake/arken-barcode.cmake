option( ARKEN_BARCODE "enable arken.barcode" false )

if( ARKEN_BARCODE )

  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/barcode/*.cpp)
  set( arken-barcode ${files} )

  include_directories(
    ${PROJECT_SOURCE_DIR}/deps/zxing/include/ZXing
    ${PROJECT_SOURCE_DIR}/deps/zxing/include/stb
  )

  link_directories( ${PROJECT_SOURCE_DIR}/deps/zxing/lib/ )

endif()

message("ARKEN_BARCODE...............: ${ARKEN_BARCODE}")
