#include <stdio.h>

#include <arken/doc/pdf.h>
#include <hpdf.h>

void error_handler (HPDF_STATUS error_no, HPDF_STATUS detail_no, void *data)
{
  printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
    (HPDF_UINT)detail_no);
}


namespace arken {
namespace doc {

  Pdf::Pdf()
  {
    auto * pdf  = new HPDF_Doc;
    auto * page = new HPDF_Page;

    *pdf  = HPDF_New(error_handler, nullptr);
    *page = HPDF_AddPage(*pdf);

    // FONT DEFAULT
    HPDF_Font def_font = HPDF_GetFont(*pdf, "Helvetica", nullptr);
    HPDF_Page_SetFontAndSize(*page, def_font, 10);

    m_doc  = std::unique_ptr<HPDF_Doc>(pdf);
    m_page = std::unique_ptr<HPDF_Page>(page);

  }

  Pdf::~Pdf() = default;

  void Pdf::write(double width, double height, string text)
  {
    HPDF_Page_BeginText(*m_page);
    HPDF_Page_TextOut(*m_page, width, height, text.data());
    HPDF_Page_EndText(*m_page);
  }

  void Pdf::writeText(double width, double height, string text)
  {
    HPDF_Page_BeginText(*m_page);
    HPDF_Page_TextOut(*m_page, width, height, text.data());
    HPDF_Page_EndText(*m_page);
  }

  void Pdf::writeRectangle(float x, float y, float width, float height, float border)
  {
    HPDF_Page_SetLineWidth(*m_page, border);
    HPDF_Page_Rectangle(*m_page, x, y, width, height);
    HPDF_Page_Stroke(*m_page);
  }

  void Pdf::setFont(string name, int size)
  {
    HPDF_Font font = HPDF_GetFont(*m_doc, name, nullptr);
    HPDF_Page_SetFontAndSize(*m_page, font, size);
  }

  void Pdf::save(string fileName)
  {
    HPDF_SaveToFile(*m_doc, fileName.data());
  }

  float Pdf::width()
  {
    return HPDF_Page_GetWidth(*m_page);
  }


  float Pdf::height()
  {
    return HPDF_Page_GetHeight(*m_page);
  }

} // namespace doc
} // namespace arken
