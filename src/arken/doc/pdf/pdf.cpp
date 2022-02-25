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
    HPDF_Doc  * pdf  = new HPDF_Doc;
    HPDF_Page * page = new HPDF_Page;

    *pdf  = HPDF_New(error_handler, NULL);
    *page = HPDF_AddPage(*pdf);

    // FONT DEFAULT
    HPDF_Font def_font = HPDF_GetFont (*pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize (*page, def_font, 10);

    m_resource  = (void*) pdf;
    m_page      = (void*) page;

  }

  Pdf::~Pdf()
  {
    free(m_resource);
    free(m_page);
  }

  void Pdf::text(double width, double height, string text)
  {
    HPDF_Page * page = (HPDF_Page *) m_page;

    HPDF_Page_BeginText(*page);
    HPDF_Page_TextOut(*page, width, height, text.data());
    HPDF_Page_EndText(*page);
  }

  void Pdf::save(string fileName)
  {
    HPDF_Doc  * pdf = (HPDF_Doc  *) m_resource;
    HPDF_SaveToFile(*pdf, fileName.data());
  }

} // namespace doc
} // namespace arken
