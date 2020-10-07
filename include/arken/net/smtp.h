// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// https://curl.haxx.se/libcurl/c/smtp-mail.html
// https://curl.haxx.se/libcurl/c/smtp-ssl.html
// https://curl-library.cool.haxx.narkive.com/AIZ014wK/smtp-authentication-fails
// https://www.bookstack.cn/read/everything-curl/usingcurl-smtp.md
// https://www.tutorialspoint.com/python/python_sending_email.htm

#ifndef _ARKEN_NET_SMTP_
#define _ARKEN_NET_SMTP_

#include <vector>
#include <arken/os.h>
#include <arken/string.h>

using string = arken::string;

namespace arken {
namespace net {

class SMTP
{
  private:

  string m_url;
  string m_from;
  string m_from_mail;
  string m_to;
  string m_to_mail;
  string m_reply_to;
  string m_reply_to_mail;
  string m_copy;
  string m_copy_mail;
  string m_subject;
  string m_body;
  string m_username;
  string m_password;
  string m_domain;
  string m_contentType = "text/plain";
  string m_message;

  unsigned int m_lines = 0;
  int  m_port = 587;
  bool m_ssl;
  bool m_verbose;
  bool m_success;

  std::vector<string> m_payload_text;

  static
  size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
  void loadText();

  public:
  SMTP(const char * url);
  bool perform();
  void setFrom(const char * mail, const char * name = nullptr);
  void setTo(const char * mail, const char * name = nullptr);
  void setCopy(const char * mail, const char * name = nullptr);
  void setReplyTo(const char * mail);
  void setSubject(const char * subject);
  void setPort(int port);
  void setSsl(bool ssl);
  void setDomain(const char * domain);
  void setBody(const char * body);
  void setVerbose(bool verbose);
  void setUsername(const char * username);
  void setPassword(const char * password);
  void setContentType(const char * contentType);
  const char * message();
  string raw();
  static
  string rfc2822Date();

};

}}
#endif
