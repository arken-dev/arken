option(ARKEN_GRAPHICS_CHART "enable arken.graphics.chart" false)
set(IMAGEMAGICK6_ROOT "" CACHE PATH "Prefix of a standalone ImageMagick 6 install (e.g. /opt/objectdata/ImageMagic-6). If empty, the system ImageMagick is located via find_package(ImageMagick).")

if(ARKEN_GRAPHICS_CHART)

  file(GLOB files
    ${PROJECT_SOURCE_DIR}/src/arken/graphics/chart.cpp
    ${PROJECT_SOURCE_DIR}/src/arken/graphics/chart/*.cpp
  )
  set(arken-graphics-chart ${files})

  if(IMAGEMAGICK6_ROOT)
    message("IMAGEMAGICK6_ROOT...........: ${IMAGEMAGICK6_ROOT}")
    set(ImageMagick_INCLUDE_DIRS ${IMAGEMAGICK6_ROOT}/include/ImageMagick-6)
    set(ImageMagick_MagickWand_INCLUDE_DIRS ${IMAGEMAGICK6_ROOT}/include/ImageMagick-6)
    set(ImageMagick_MagickWand_LIBRARY ${IMAGEMAGICK6_ROOT}/lib/libMagickWand-6.Q16.so)
    set(ImageMagick_LIBRARIES ${IMAGEMAGICK6_ROOT}/lib/libMagickWand-6.Q16.so ${IMAGEMAGICK6_ROOT}/lib/libMagickCore-6.Q16.so)
    add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=0)
    add_definitions(-DIMAGEMAGICK_VERSION=6)
  else()
    find_package(ImageMagick
      REQUIRED
      COMPONENTS MagickWand
    )
    if(ImageMagick_VERSION_STRING VERSION_LESS 7.0.0)
      add_definitions(-DIMAGEMAGICK_VERSION=6)
    else()
      add_definitions(-DIMAGEMAGICK_VERSION=7)
    endif()

    # find_package(ImageMagick) do CMake nao expoe MAGICKCORE_QUANTUM_DEPTH/
    # MAGICKCORE_HDRI_ENABLE, mas magick-config.h exige os dois definidos.
    # Pega os defines certos direto do .pc da lib instalada (nome varia por
    # empacotamento: MagickWand, MagickWand-7.Q16HDRI, etc).
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
      execute_process(
        COMMAND pkg-config --list-all
        COMMAND grep -m1 -o "^MagickWand[^ ]*"
        OUTPUT_VARIABLE ARKEN_IM_PKG_NAME
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
      )
      if(ARKEN_IM_PKG_NAME)
        pkg_check_modules(ARKEN_IM_PKG QUIET ${ARKEN_IM_PKG_NAME})
        add_definitions(${ARKEN_IM_PKG_CFLAGS_OTHER})
      else()
        message(WARNING "Nao achei .pc do MagickWand via pkg-config; se a build falhar em magick-config.h, defina manualmente MAGICKCORE_QUANTUM_DEPTH/MAGICKCORE_HDRI_ENABLE (ex: -DCMAKE_CXX_FLAGS=\"-DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=1\").")
      endif()
    endif()
  endif()

  include_directories(
    ${ImageMagick_INCLUDE_DIRS}
    ${ImageMagick_MagickWand_INCLUDE_DIRS}
  )

endif()

message("ARKEN_GRAPHICS_CHART........: ${ARKEN_GRAPHICS_CHART}")
