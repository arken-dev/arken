option(ARKEN_DOC_PDF "enable arken.doc.Pdf" false)

if(ARKEN_DOC_PDF)
  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/doc/pdf/*.cpp)
  set(arken-doc-pdf ${files})
endif()

message("ARKEN_DOC_PDF...............: ${ARKEN_DOC_PDF}")
