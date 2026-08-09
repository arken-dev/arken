#include <arken/barcode.h>

#include <zint.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <stdexcept>
#include <string>


namespace arken {

namespace {

// Compat layer: keeps the ZXing BarcodeFormat names arken.Barcode has always
// accepted (see https://github.com/nu-book/zxing-cpp BarcodeFormat.h) mapped
// onto zint's BARCODE_* symbologies.
struct FormatEntry { const char * name; int symbology; };

const FormatEntry formatTable[] = {
  { "Codabar",        BARCODE_CODABAR     },
  { "Code39",         BARCODE_CODE39      },
  { "Code39Ext",      BARCODE_EXCODE39    },
  { "Code93",         BARCODE_CODE93      },
  { "Code128",        BARCODE_CODE128     },
  { "ITF",            BARCODE_C25INTER    },
  { "ITF14",          BARCODE_ITF14       },
  { "EAN13",          BARCODE_EANX        },
  { "EAN8",           BARCODE_EANX        },
  { "UPCA",           BARCODE_UPCA        },
  { "UPCE",           BARCODE_UPCE        },
  { "PDF417",         BARCODE_PDF417      },
  { "CompactPDF417",  BARCODE_PDF417COMP  },
  { "MicroPDF417",    BARCODE_MICROPDF417 },
  { "Aztec",          BARCODE_AZTEC       },
  { "AztecCode",      BARCODE_AZTEC       },
  { "QRCode",         BARCODE_QRCODE      },
  { "QRCodeModel2",   BARCODE_QRCODE      },
  { "MicroQRCode",    BARCODE_MICROQR     },
  { "DataMatrix",     BARCODE_DATAMATRIX  },
  { "MaxiCode",       BARCODE_MAXICODE    },
  { "DataBar",        BARCODE_DBAR_OMN    },
  { "DataBarOmni",    BARCODE_DBAR_OMN    },
  { "DataBarStk",     BARCODE_DBAR_STK    },
  { "DataBarStkOmni", BARCODE_DBAR_OMNSTK },
  { "DataBarLtd",     BARCODE_DBAR_LTD    },
  { "DataBarExp",     BARCODE_DBAR_EXP    },
  { "DataBarExpStk",  BARCODE_DBAR_EXPSTK },
  { "Telepen",        BARCODE_TELEPEN     },
  { "TelepenAlpha",   BARCODE_TELEPEN     },
  { "TelepenNumeric", BARCODE_TELEPEN_NUM },
};

bool
equalsIgnoreCase(const char * a, const char * b)
{
  while (*a && *b)
    if (std::tolower((unsigned char)*a++) != std::tolower((unsigned char)*b++))
      return false;
  return *a == *b;
}

int
symbologyFromFormat(const char * format)
{
  for (const auto & entry : formatTable)
    if (equalsIgnoreCase(format, entry.name))
      return entry.symbology;

  throw std::invalid_argument(std::string("This is not a valid barcode format: '") + format + "'");
}

} // namespace


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
  int symbology = symbologyFromFormat(m_format.data());

  zint_symbol * symbol = ZBarcode_Create();
  if (!symbol)
    throw std::runtime_error("zint: failed to create symbol");

  symbol->symbology = symbology;

  // DataMatrix defaults to whichever symbol size (square or rectangular)
  // fits the data smallest, which can produce a visibly non-square symbol.
  // Force square versions, since that's the expected shape almost always.
  if (symbology == BARCODE_DATAMATRIX)
    symbol->option_3 = DM_SQUARE;

  // Pass 1: encode at the default scale just to measure the symbol's
  // natural size, so pass 2 can pick a scale that fits (m_width, m_height).
  int error = ZBarcode_Encode_and_Buffer(
    symbol, (const unsigned char *)m_text.data(), 0, 0
  );
  if (error >= ZINT_ERROR) {
    std::string message = symbol->errtxt;
    ZBarcode_Delete(symbol);
    throw std::runtime_error(message);
  }

  bool fixedRatio = ZBarcode_Cap(symbology, ZINT_CAP_FIXED_RATIO) != 0;

  if (fixedRatio) {
    // 2D/matrix symbologies: width and height scale together, so fit the
    // largest requested dimension against the largest natural one.
    int naturalSize = std::max(symbol->bitmap_width, symbol->bitmap_height);
    int targetSize  = std::max(m_width, m_height);
    if (naturalSize > 0 && targetSize > 0)
      symbol->scale = float(targetSize) / float(naturalSize);
  } else if (symbol->bitmap_width > 0) {
    // Linear symbologies: width and bar height can be fit independently.
    // zint renders 2 pixels per X-dimension unit of `height`, plus a fixed
    // offset (quiet zones, HRT, ...) that pass 1 already measured at its
    // effective default height (zint writes it back into symbol->height).
    float naturalHeightField = symbol->height;
    float offset = symbol->bitmap_height - 2.0f * naturalHeightField;

    symbol->scale = float(m_width) / float(symbol->bitmap_width);
    if (symbol->scale > 0)
      symbol->height = std::max(1.0f, (float(m_height) / symbol->scale - offset) / 2.0f);
  }

  std::strncpy(symbol->outfile, path.data(), sizeof(symbol->outfile) - 1);
  symbol->outfile[sizeof(symbol->outfile) - 1] = '\0';

  // Pass 2: encode again at the fitted scale and write the PNG.
  error = ZBarcode_Encode_and_Print(
    symbol, (const unsigned char *)m_text.data(), 0, 0
  );
  if (error >= ZINT_ERROR) {
    std::string message = symbol->errtxt;
    ZBarcode_Delete(symbol);
    throw std::runtime_error(message);
  }

  ZBarcode_Delete(symbol);
}

} // namespace arken
