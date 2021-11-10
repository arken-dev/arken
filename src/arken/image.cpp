#include <arken/image.h>
#include <wand/magick_wand.h>

namespace arken {

Image::Image(const char * path)
{
  m_path = path;
}

void Image::save(const char * path, int quality)
{
  MagickWand *mw = NULL;

  MagickWandGenesis();

  /* Create a wand */
  mw = NewMagickWand();

  /* Read the input image */
  MagickReadImage(mw, m_path);

  /* write it */
  MagickWriteImage(mw, path);

  /* Tidy up */
  if(mw) mw = DestroyMagickWand(mw);

  MagickWandTerminus();
}

} // namespace
