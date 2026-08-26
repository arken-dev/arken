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

      if(NOT zxing-root AND ARKEN_BARCODE_BACKEND STREQUAL "zxing-legacy")
        # a legacy install often just sits under a directory named "zxing"
        # rather than "zxing-legacy" (that's how auto-detection finds it
        # in the first place). Accept it here too, but only once verified
        # as the old API, same check as the auto-detect block above.
        arken_vendor_root(zxing-fallback-root zxing)
        if(zxing-fallback-root AND (
             EXISTS "${zxing-fallback-root}/include/MultiFormatWriter.h" OR
             EXISTS "${zxing-fallback-root}/include/ZXing/MultiFormatWriter.h"))
          set(zxing-root ${zxing-fallback-root})
        endif()
      endif()
    endif()

    if(zxing-root)
      message("ZXING root...................: ${zxing-root}")
      set(CMAKE_PREFIX_PATH "${zxing-root};${CMAKE_PREFIX_PATH}")
    endif()

    # ZXing::ZXing's exported link interface pulls in Threads::Threads.
    # That target only gets created elsewhere in this project when
    # ARKEN_NET_HTTPSERVER/ARKEN_MVM_CONTAINER use boost, so make sure it
    # exists here too or linking ZXing::ZXing fails ("-lThreads::Threads").
    find_package(Threads REQUIRED)
    find_package(ZXing CONFIG REQUIRED)

    # Link against the ZXing::ZXing imported target directly (arken-graphics
    # links plain lib names like "ZXing" via link_directories, but that
    # relies on reading back IMPORTED_LOCATION_<CONFIG> ourselves to derive
    # a -L dir, and the config suffix — NOCONFIG, RELEASE, ... — varies by
    # how ZXing was built, so it silently breaks on some installs). Linking
    # the imported target sidesteps all of that: CMake resolves the actual
    # library file itself, whatever it's named/configured as.
    get_target_property(ZXING_INCLUDE_DIR ZXing::ZXing INTERFACE_INCLUDE_DIRECTORIES)

    include_directories(
      ${ZXING_INCLUDE_DIR}
      ${ZXING_INCLUDE_DIR}/ZXing
      ${PROJECT_SOURCE_DIR}/src/vendors/stb
    )

  endif()

endif()

message("ARKEN_BARCODE...............: ${ARKEN_BARCODE}")
message("ARKEN_BARCODE_BACKEND.......: ${ARKEN_BARCODE_BACKEND} (zint|zxing|zxing-legacy)")
