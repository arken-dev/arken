option(ARKEN_GRAPHICS_CHART "enable arken.graphics.Chart" false)
set(IMAGEMAGICK6_ROOT "" CACHE PATH "Prefix of a standalone ImageMagick 6 install (e.g. /opt/objectdata/ImageMagic-6). If empty, the system ImageMagick is located via find_package(ImageMagick).")

if(ARKEN_GRAPHICS_CHART)

  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/graphics/chart/*.cpp)
  set(arken-graphics-chart ${files})

  if(IMAGEMAGICK6_ROOT)
    message("IMAGEMAGICK6_ROOT...........: ${IMAGEMAGICK6_ROOT}")
    set(ImageMagick_INCLUDE_DIRS ${IMAGEMAGICK6_ROOT}/include/ImageMagick-6)
    set(ImageMagick_MagickWand_INCLUDE_DIRS ${IMAGEMAGICK6_ROOT}/include/ImageMagick-6)
    set(ImageMagick_MagickWand_LIBRARY ${IMAGEMAGICK6_ROOT}/lib/libMagickWand-6.Q16.so)
    set(ImageMagick_LIBRARIES ${IMAGEMAGICK6_ROOT}/lib/libMagickWand-6.Q16.so ${IMAGEMAGICK6_ROOT}/lib/libMagickCore-6.Q16.so)
    add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=0)
  else()
    find_package(ImageMagick
      REQUIRED
      COMPONENTS MagickWand
    )
  endif()

  include_directories(
    ${ImageMagick_INCLUDE_DIRS}
    ${ImageMagick_MagickWand_INCLUDE_DIRS}
  )

endif()

message("ARKEN_GRAPHICS_CHART........: ${ARKEN_GRAPHICS_CHART}")
