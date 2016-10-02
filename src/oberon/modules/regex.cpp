#include <oberon/helper>
#include <oberon/modules/regex.h>
#include <QRegExp>
#include <QString>
#include <QByteArray>
#include <cstring>


bool regex::match(const char * string, const char * regex)
{
  return QRegExp(regex).exactMatch(string);
}

char * regex::replace(const char * string, const char * regex, const char * after)
{
  QString str(string);
  QString aft = str.replace(QRegExp(regex), after);
  char * result = new char[aft.size() + 1];
  strcpy(result, aft.toLocal8Bit().data());
  return result;
}
