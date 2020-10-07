#include <arken/net/smtp.h>
#include <curl/curl.h>
#include <time.h>
#include <ctime>
#include <memory>

using namespace arken::net;

size_t SMTP::payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  SMTP *upload_ctx = (SMTP *)userp;
  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = upload_ctx->m_payload_text[upload_ctx->m_lines];

  if( upload_ctx->m_lines < upload_ctx->m_payload_text.size() ) {
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->m_lines++;

    return len;
  }

  return 0;
}

SMTP::SMTP(const char * url)
{
  m_url = url;
}

void SMTP::loadText()
{

  if( m_payload_text.size() == 0 ) {
    m_payload_text.push_back( string("Date: ").append(SMTP::rfc2822Date()).append("\r\n") );
    m_payload_text.push_back( string("From: ").append(m_from).append("\r\n") );

    if( m_to_mail.contains(";") ) {
      List * list = m_to_mail.split(";");
      for(int i=0; i < list->size(); i++) {
        m_payload_text.push_back( string("To: ").append(list->at(i)).append("\r\n") );
      }
      delete list;
    } else {
      m_payload_text.push_back( string("To: ").append(m_to).append("\r\n") );
    }

    if( ! m_reply_to.empty() ) {
      m_payload_text.push_back( string("Reply-To: ").append(m_reply_to).append("\r\n") );
    }

    if(! m_copy.empty() ) {
      m_payload_text.push_back( string("Cc: ").append(m_copy).append("\r\n") );
    }

    //m_payload_text.push_back( "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@rfcpedant.example.org>\r\n" );
    m_payload_text.push_back( string("Message-ID: <").append(os::uuid()).append("@").append(m_domain).append(">\r\n") );

    if(! m_subject.empty() ) {
      m_payload_text.push_back( string("Subject: ").append(m_subject).append("\r\n") );
    }

    //m_payload_text.push_back( "Content-Type: text/plain\r\n" );
    m_payload_text.push_back( string("Content-Type: ").append(m_contentType).append("\r\n") );

    /* empty line to divide headers from body, see RFC5322 */
    m_payload_text.push_back( "\r\n" );

    /* body */
    m_payload_text.push_back( string(m_body).append("\r\n") );

    //m_payload_text.push_back("\r\n");
    //m_payload_text.push_back("It could be a lot of lines, could be MIME encoded, whatever.\r\n");
    //m_payload_text.push_back("Check RFC5322.\r\n");
  }
}

bool SMTP::perform()
{

  CURL * curl        = curl_easy_init();
  curl_slist * slist = nullptr;
  CURLcode cresult   = CURLE_OK;
  m_success          = true;

  if(curl) {

    loadText();

    if( m_verbose ) {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    } else {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    }

    /* This is the URL for your mailserver */
    string url;
    if( m_ssl ) {
      url.append("smtps://");
      curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    } else {
      url.append("smtp://");
    }

    url.append(m_url).append(":").append(std::to_string(m_port));
    curl_easy_setopt(curl, CURLOPT_URL, m_url.data());

    // ???
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_NONE);

    if(! m_username.empty() ) {
      curl_easy_setopt(curl, CURLOPT_USERNAME, m_username.data());
    }

    if(! m_password.empty() ) {
      curl_easy_setopt(curl, CURLOPT_PASSWORD, m_password.data());
    }

     /*
     curl_easy_setopt(curl, CURLOPT_SSLVERSION, 0);
     curl_easy_setopt(curl, CURLOPT_SSL_SESSIONID_CACHE, 0);
     */

    /* Note that this option isn't strictly required, omitting it will result
     * in libcurl sending the MAIL FROM command with empty sender data. All
     * autoresponses should have an empty reverse-path, and should be directed
     * to the address in the reverse-path which triggered them. Otherwise,
     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
     * details.
     */
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, m_from_mail.data());

    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
    if( m_to_mail.contains(";") ) {
      List * list = m_to_mail.split(";");
      for(int i=0; i < list->size(); i++) {
        slist = curl_slist_append(slist, list->at(i));
      }
      delete list;
    } else {
      slist = curl_slist_append(slist, m_to_mail.data());
    }
    if(! m_copy_mail.empty()) {
      slist = curl_slist_append(slist, m_copy_mail.data());
    }
    if(! m_reply_to_mail.empty()) {
      slist = curl_slist_append(slist, m_reply_to_mail.data());
    }
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, slist);

    /* We're using a callback function to specify the payload (the headers and
     * body of the message). You could just use the CURLOPT_READDATA option to
     * specify a FILE pointer to read from. */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, SMTP::payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, this);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* Send the message */
    cresult = curl_easy_perform(curl);

    /* Check for errors */
    if(cresult != CURLE_OK) {
      m_success = false;
      m_message = curl_easy_strerror(cresult);
    }

    /* Free the list of recipients */
    curl_slist_free_all(slist);

    /* curl won't send the QUIT command until you call cleanup, so you should
     * be able to re-use this connection for additional messages (setting
     * CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and calling
     * curl_easy_perform() again. It may not be a good idea to keep the
     * connection open for a very long time though (more than a few minutes
     * may result in the server timing out the connection), and you do want to
     * clean up in the end.
     */
    curl_easy_cleanup(curl);

    m_payload_text.clear();

    return m_success;
  } else {
    m_success = false;
    m_message = "could not alocate memory";
  }

  return m_success;
}

void SMTP::setFrom(const char * mail, const char * name)
{

  m_from_mail = mail;

  if( name ) {
    m_from.append(name);
    m_from.append(" ");
  }

  m_from.append("<");
  m_from.append(mail);
  m_from.append(">");
}

void SMTP::setTo(const char * mail, const char * name)
{

  m_to_mail = mail;

  if( name ) {
    m_to.append(name);
    m_to.append(" ");
  }

  m_to.append("<");
  m_to.append(mail);
  m_to.append(">");
}

void SMTP::setReplyTo(const char * mail)
{
  m_reply_to_mail = mail;

  m_reply_to.append("<");
  m_reply_to.append(mail);
  m_reply_to.append(">");
}


void SMTP::setCopy(const char * mail, const char * name)
{

  m_copy_mail = mail;

  if( name ) {
    m_copy.append(name);
    m_copy.append(" ");
  }

  m_copy.append("<");
  m_copy.append(mail);
  m_copy.append(">");
}

void SMTP::setSubject(const char * subject)
{
  m_subject = subject;
}

void SMTP::setPort(int port)
{
  m_port = port;
}

void SMTP::setSsl(bool ssl)
{
  m_ssl = ssl;
}

void SMTP::setDomain(const char * domain)
{
  m_domain = domain;
}

void SMTP::setBody(const char * body)
{
  m_body = body;
}

void SMTP::setVerbose(bool verbose)
{
  m_verbose = verbose;
}

void SMTP::setUsername(const char * username)
{
  m_username = username;
}

void SMTP::setPassword(const char * password)
{
  m_password = password;
}

void SMTP::setContentType(const char * contentType)
{
  m_contentType = contentType;
}

string SMTP::rfc2822Date()
{
  time_t rawtime;
  struct tm * timeinfo;
  string buffer(80);// = new char[80];

  time(&rawtime);
  timeinfo = localtime (&rawtime);

  strftime(buffer.data(), 80, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

  return buffer;
}

string SMTP::raw()
{
  string result;

  loadText();

  for(unsigned int i=0; i <   m_payload_text.size(); i++) {
    result.append(m_payload_text[i]);
  }

  return result;
}

const char * SMTP::message()
{
  return m_message.data();
}
