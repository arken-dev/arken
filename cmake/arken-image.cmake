option(ARKEN_IMAGE "enable arken.Image" false)

if(ARKEN_IMAGE)

  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/image/*.cpp)
  set( arken-image ${files} )

  find_package(ImageMagick
    REQUIRED
    COMPONENTS MagickWand
  )

  include_directories(
    ${ImageMagick_INCLUDE_DIRS}
    ${ImageMagick_MagickWand_INCLUDE_DIRS}
  )

endif()

message("ARKEN IMAGE.............: ${ARKEN_IMAGE}")
