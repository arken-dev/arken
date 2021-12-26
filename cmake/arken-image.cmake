option(ARKEN_IMAGE "enable arken.image" true)

if(ARKEN_IMAGE)

  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/image/*.cpp)
  set( arken-image ${files} )

  FIND_PACKAGE(ImageMagick
    REQUIRED
    COMPONENTS MagickWand
  )

  INCLUDE_DIRECTORIES(
    ${ImageMagick_INCLUDE_DIRS}
    ${ImageMagick_MagickWand_INCLUDE_DIRS}
  )

endif()

message("ARKEN IMAGE.............: ${ARKEN_IMAGE}")
