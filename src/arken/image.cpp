#include <arken/image.h>

namespace arken {

Image::Image(int width, int height, string color)
{
  m_width  = width;
  m_height = height;

  MagickWandGenesis();

  /* Create a wand */
  mw = NewMagickWand();
  PixelWand *pmw = NewPixelWand();
  PixelSetColor(pmw, color);
  MagickNewImage (mw, (unsigned long) width, (unsigned long) height, pmw);
}

Image::Image(const Image &obj)
{
  m_width  = obj.m_width;
  m_height = obj.m_height;
  m_path   = obj.m_path;
}

Image::Image(const char * path)
{
  m_width  = -1;
  m_height = -1;
  m_path   = path;

  MagickWandGenesis();

  /* Create a wand */
  mw = NewMagickWand();

  /* Read the input image */
  MagickReadImage(mw, m_path);
}

Image::~Image()
{
  /* Tidy up */
  if(mw) mw = DestroyMagickWand(mw);

  MagickWandTerminus();
}

void Image::resize(int width, int height)
{
  m_width  = width;
  m_height = height;

  MagickResizeImage(mw, (unsigned long) m_width, (unsigned long) m_height, BoxFilter, 1.0);
}

int Image::width()
{
  return (int) MagickGetImageWidth(mw);
}

int Image::height()
{
  return (int) MagickGetImageHeight(mw);
}

void Image::save(const char * path, int quality)
{

  if( quality > 0 ) {
    MagickSetImageCompressionQuality(mw, quality);
  }

  /* write it */
  MagickWriteImage(mw, path);
}

void Image::composite(Image * img)
{
  size_t x = ( this->width()  - img->width()  ) / 2;
  size_t y = ( this->height() - img->height() ) / 2;
  MagickCompositeImage(mw, img->mw, OverCompositeOp, x, y);
}

void Image::composite(Image * img, size_t x, size_t y)
{
  MagickCompositeImage(mw, img->mw, OverCompositeOp, x, y);
}


} // namespace
