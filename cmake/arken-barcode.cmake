option( ARKEN_BARCODE "enable arken.barcode" false )

set(ARKEN_BARCODE_BACKEND "" CACHE STRING "arken barcode backend. Empty: auto-detected from ARKEN_VENDOR_ROOT (priority zint > zxing > zxing-legacy), falling back to zint if nothing is found there. Set explicitly to skip auto-detection entirely.")

set_property(
  CACHE ARKEN_BARCODE_BACKEND PROPERTY STRINGS "" zint zxing zxing-legacy
)

set(ZXING_ROOT "" CACHE PATH "Explicit prefix of a standalone ZXing-C++ install (e.g. /opt/objectdata/zxing), used by the zxing/zxing-legacy backends. Takes priority over ARKEN_VENDOR_ROOT. If empty, \${ARKEN_VENDOR_ROOT}/\${ARKEN_BARCODE_BACKEND} is tried, then the system ZXing via find_package(ZXing).")
set(ZINT_ROOT "" CACHE PATH "Explicit prefix of a standalone libzint install, used by the zint backend. Takes priority over ARKEN_VENDOR_ROOT. If empty, \${ARKEN_VENDOR_ROOT}/zint is tried, then the system libzint-dev package.")

if( ARKEN_BARCODE )

  # ---------------------------------------------------------------------
  # Backend auto-detection: only runs when ARKEN_BARCODE_BACKEND was not
  # set explicitly. Scans ARKEN_VENDOR_ROOT, priority zint > zxing >
  # zxing-legacy. A directory literally named "zxing" can hold either API
  # generation depending on when it was installed, so it's disambiguated
  # by probing for a header unique to the old (legacy) API.
  # ---------------------------------------------------------------------
  if(NOT ARKEN_BARCODE_BACKEND)

    arken_vendor_root(detected-zint-root zint)
    arken_vendor_root(detected-zxing-root zxing)
    arken_vendor_root(detected-zxing-legacy-root zxing-legacy)

    if(detected-zint-root)

      set(ARKEN_BARCODE_BACKEND zint)
      set(ZINT_ROOT ${detected-zint-root})

    elseif(detected-zxing-root)

      if(EXISTS "${detected-zxing-root}/include/MultiFormatWriter.h" OR
         EXISTS "${detected-zxing-root}/include/ZXing/MultiFormatWriter.h")
        set(ARKEN_BARCODE_BACKEND zxing-legacy)
      else()
        set(ARKEN_BARCODE_BACKEND zxing)
      endif()
      set(ZXING_ROOT ${detected-zxing-root})

    elseif(detected-zxing-legacy-root)

      set(ARKEN_BARCODE_BACKEND zxing-legacy)
      set(ZXING_ROOT ${detected-zxing-legacy-root})

    else()

      # nothing under ARKEN_VENDOR_ROOT; zint also works from the system
      # package alone (Debian/Ubuntu: libzint-dev), so it's the safest
      # backend to fall back to.
      set(ARKEN_BARCODE_BACKEND zint)

    endif()

    message("ARKEN_BARCODE_BACKEND (auto): ${ARKEN_BARCODE_BACKEND}")

  endif()

  set( arken-barcode ${PROJECT_SOURCE_DIR}/src/arken/barcode/${ARKEN_BARCODE_BACKEND}.cpp )

  if(ARKEN_BARCODE_BACKEND STREQUAL "zint")

    if(ZINT_ROOT)
      set(zint-root ${ZINT_ROOT})
    else()
      arken_vendor_root(zint-root zint)
    endif()

    if(zint-root)
      message("ZINT root....................: ${zint-root}")
      include_directories( ${zint-root}/include )
      link_directories( ${zint-root}/lib )
    endif()
    # else: system package (Debian/Ubuntu: libzint-dev), default search paths.

  else() # zxing | zxing-legacy

    if(ZXING_ROOT)
      set(zxing-root ${ZXING_ROOT})
    else()
      arken_vendor_root(zxing-root ${ARKEN_BARCODE_BACKEND})
    endif()

    if(zxing-root)
      message("ZXING root...................: ${zxing-root}")
      set(CMAKE_PREFIX_PATH "${zxing-root};${CMAKE_PREFIX_PATH}")
    endif()

    find_package(ZXing CONFIG REQUIRED)

    get_target_property(ZXING_INCLUDE_DIR ZXing::ZXing INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(ZXING_LIB_PATH ZXing::ZXing IMPORTED_LOCATION_NOCONFIG)
    get_filename_component(ZXING_LIB_DIR ${ZXING_LIB_PATH} DIRECTORY)

    include_directories(
      ${ZXING_INCLUDE_DIR}
      ${PROJECT_SOURCE_DIR}/src/vendors/stb
    )

    link_directories( ${ZXING_LIB_DIR} )

  endif()

endif()

message("ARKEN_BARCODE...............: ${ARKEN_BARCODE}")
message("ARKEN_BARCODE_BACKEND.......: ${ARKEN_BARCODE_BACKEND} (zint|zxing|zxing-legacy)")
