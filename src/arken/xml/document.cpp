// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/xml/document.h>

// DEBUG USE
static void
print_element_names(xmlNode * a_node)
{
  xmlNode *cur_node = NULL;

  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      printf("node type: Element, name: %s\n", cur_node->name);
    }

    print_element_names(cur_node->children);
  }
}

// https://qnaplus.com/search-for-an-xml-node-using-libxml2-in-c/
xmlNode * find_node(xmlNode * node, const char * node_name) {

  xmlNode * result;

  if (node == NULL) return NULL;
  //printf("find_node: Element, name: %s\n", node->name);

  while(node) {
    if((node->type == XML_ELEMENT_NODE)
        && (strcmp((char*)node->name, node_name) == 0)) {
      //printf("achei: Element, name: %s\n", node->name);
      return node;
    }

    result = find_node(node->children, node_name);

    if(result) {
      //printf("dentro do if: %s\n", node->name);
      return result;
    }

    node = node->next;
  }

  return NULL;
}

namespace arken {
namespace xml {

Node::Node(xmlDocPtr doc, const char * name, const char * value) {
  m_node = xmlNewDocNode(doc, NULL, (xmlChar *) name, (xmlChar *) value);
}

Node::Node(xmlNode * node) {
  m_node = node;
}

void Node::append(Node * node) {
  xmlAddChild(m_node, node->m_node);
}

void Node::attribute(const char *key, const char *value) {
  xmlNewProp(m_node, BAD_CAST key, BAD_CAST value);
}

void Node::setContent(const char *content) {
  xmlNodeSetContent(m_node, BAD_CAST content);
}

Document::Document() {
  m_ctx = xmlNewParserCtxt();
}

Document::~Document() {
  xmlFreeParserCtxt(m_ctx);
}

void Document::loadFile(const char *path)
{
  m_doc  = xmlCtxtReadFile(m_ctx, path, NULL, XML_PARSE_NOERROR | XML_PARSE_NOBLANKS);
  m_root = xmlDocGetRootElement(m_doc);
}

void Document::loadXml(string buffer)
{
  m_doc  = xmlCtxtReadMemory(m_ctx, buffer.data(), buffer.size(), NULL, NULL, XML_PARSE_NOERROR | XML_PARSE_NOBLANKS);
  m_root = xmlDocGetRootElement(m_doc);
}

string Document::canonicalize()
{
  xmlOutputBufferPtr buf = xmlAllocOutputBuffer(NULL);
  xmlC14NDocSaveTo(m_doc, 0, 0, 0, 0, buf);
  size_t size  = xmlOutputBufferGetSize(buf);
  string result = string((char *) xmlOutputBufferGetContent(buf), size);
  // clear buf ??? double free error
  // xmlOutputBufferClose(buf);
  return result;
}

string Document::c14n()
{
  return this->canonicalize();
}

//size_t size = xmlSaveFile("/tmp/save.xml", m_doc);
string Document::save()
{
  // debug
  //print_element_names(m_root);
  int size;
  xmlChar * buf;
  xmlDocDumpFormatMemory(m_doc, &buf, &size, 1);
  return string((char *) buf, size);
}

Node * Document::create(const char * name, const char * value)
{
  return new Node(m_doc, name, value);
}

Node * Document::search(const char * name)
{
  xmlNode * node = find_node(m_root, name);
  if( node == nullptr) {
    return nullptr;
  } else {
    return new Node(node);
  }
}

xmlNode * Node::value()
{
  return m_node;
}

Node * Document::root()
{
  if(!m_root) {
    m_root = xmlNewNode(NULL, BAD_CAST "root");
    xmlDocSetRootElement(m_doc, m_root);
    return new Node(m_root);
  }
  return new Node(m_root);
}

void Document::append(Node * node) {
  if(!m_root) {
    m_doc = xmlNewDoc(BAD_CAST "1.0");
    m_root = node->value();
    xmlDocSetRootElement(m_doc, m_root);
  } else {
    xmlAddChild(m_root, node->value());
  }
}

string Node::c14n()
{
  xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
  xmlDocSetRootElement(doc, m_node);

  xmlOutputBufferPtr buf = xmlAllocOutputBuffer(NULL);
  xmlC14NDocSaveTo(doc, 0, 0, 0, 0, buf);
  size_t size  = xmlOutputBufferGetSize(buf);
  string result = string((char *) xmlOutputBufferGetContent(buf), size);
  // clear buf ??? double free error
  // xmlOutputBufferClose(buf);
  return result;
}

} // namespace xml
} // namespace arken
