#include <arken/barcode.h>

#include "BarcodeFormat.h"
#include "CreateBarcode.h"
#include "WriteBarcode.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>


namespace arken {

Barcode::Barcode(int width, int height, string format, string text)
{
  m_width  = width;
  m_height = height;
  m_format = format;
  m_text   = text;
}

Barcode::~Barcode()
{

}

void Barcode::setText(string text)
{
  m_text = text;
}

void Barcode::save(string path)
{
  auto format  = ZXing::BarcodeFormatFromString(m_format.data());
  auto barcode = ZXing::CreateBarcodeFromText(m_text.data(), ZXing::CreatorOptions(format));
  auto image   = ZXing::WriteBarcodeToImage(
    barcode,
    ZXing::WriterOptions().scale(-std::max(m_width, m_height)).addQuietZones(true)
  );
  stbi_write_png(path.data(), image.width(), image.height(), 1, image.data(), 0);
}

} // namespace arken
