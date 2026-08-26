#include <arken/barcode.h>

#include "BarcodeFormat.h"
#include "BitMatrix.h"
#include "MultiFormatWriter.h"
#include "TextUtfEncoding.h"
#include "CharacterSetECI.h"
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
  ZXing::CharacterSet encoding = ZXing::CharacterSet::Unknown;
  int eccLevel = -1;
  int margin   = 10;
  auto writer  = ZXing::MultiFormatWriter(ZXing::BarcodeFormatFromString(m_format.data())).
    setMargin(margin).setEncoding(encoding).setEccLevel(eccLevel);
  auto bitmap  = ZXing::ToMatrix<uint8_t>(
    writer.encode(ZXing::TextUtfEncoding::FromUtf8(m_text.data()), m_width, m_height)
  );
  stbi_write_png(path.data(), m_width, m_height, 1, bitmap.data(), 0);
}

} // namespace arken
