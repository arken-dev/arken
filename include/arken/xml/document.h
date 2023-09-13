// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// http://www.xmlsoft.org/examples/
// http://www.xmlsoft.org/examples/tree2.c
// https://stackoverflow.com/questions/13021785/finding-a-node-in-xml-file-with-libxml2

#ifndef _ARKEN_XML_DOCUMENT_
#define _ARKEN_XML_DOCUMENT_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/c14n.h>

namespace arken {
namespace xml {

class Node
{
  xmlNode * m_node;
  public:
  Node(xmlDocPtr doc, const char * name, const char * value);
  Node(xmlNode * node);
  void append(Node *);
  void attribute(const char *key, const char *value);
  xmlNode * value();
};

class Document
{
  xmlParserCtxtPtr m_ctx = NULL;
  xmlDocPtr  m_doc  = NULL;
  xmlNodePtr m_root = NULL;

  public:
  Document();
  ~Document();
  void loadFile(const char * path);
  void loadXml(string buffer);
  string canonicalize();
  string c14n();
  string save();
  Node * create(const char * name, const char * value = nullptr);
  Node * root();
  void append(Node *);
};

} // namespace xml
} // namespace arken

#endif
