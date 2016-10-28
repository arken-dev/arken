#include "dialog.h"
#include "ui_dialog.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QHash>
#include <QDir>
#include <QFile>
#include <QKeyEvent>

#define DEBUG_CONSOLE false

#include <keyloggerworker.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window
                   | Qt::WindowTitleHint
                   | Qt::CustomizeWindowHint
                   | Qt::FramelessWindowHint
                   #if defined(Q_OS_WIN)
                   | Qt::WindowStaysOnTopHint // remove porque ele não mostra uma janela em cima da outra
                   #endif
                   | Qt::SplashScreen //Ele mantem em backgroud mesmo quand otermina a aplicaçao.
                   );
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    const int width = QApplication::desktop()->width();
    this->move(width-400,0);
    QFile file(QDir::tempPath() + "/notify");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.close();
    m_bootTime = QDateTime::currentDateTime();
    QString tmp_file = (QDir::tempPath() + "/notify");
    //KeyLoggerWorker * keylogger = new KeyLoggerWorker(tmp_file);
    //keylogger->start();
}

void Dialog::mouseReleaseEvent(QMouseEvent * event){
    if(DEBUG_CONSOLE) {
      qDebug() << event ;
    }
    this->fecharAlerta();
 }

Dialog::~Dialog()
{
    delete ui;
}

//Seta a mensagem no alerta
bool Dialog::sendNotify(QString texto){
  if(DEBUG_CONSOLE) {
    qDebug() << texto;
  }
  ui->notify_label->setText(texto);
  return this->resizeMsg();
}

bool Dialog::setTitle(QString texto){
  ui->label->setText(texto);
  return true;
}
//Redimensiona a janela
bool Dialog::resizeMsg(){
    ui->notify_label->adjustSize();
    ui->label->adjustSize();

    int sizeGroup = ui->notify_label->height();
    ui->principal->move(0,0);
    ui->conteudo->setFixedHeight(sizeGroup + 80);

    ui->principal->setFixedHeight(sizeGroup + 80);
    this->setFixedHeight(sizeGroup + 80);

    return true;
}

//Inicia o contador para fechar automatico
void Dialog::showEvent ( QShowEvent * event ) {
      if(DEBUG_CONSOLE) {
          qDebug() << event;
      }
    m_currenttime = 0;
    if (m_autoclose) {
    this->startTimer(100);
    }
}


// Permanesse rodando até currentTime atingir o Timeout
// Após isto fecha aplicaçao
void Dialog::timerEvent(QTimerEvent *event)
{
    m_currenttime++;
    QFileInfo file(QDir::tempPath() + "/notify");
    if(DEBUG_CONSOLE) {
      qDebug() << m_currenttime;
      qDebug() << event;
      qDebug() << "checagem da data" << m_bootTime;
      qDebug() << "file info " << file.lastModified();
    }

    if (m_currenttime>= (m_timeout * 10) or m_bootTime < file.lastModified()) {
      this->fecharAlerta();
    }
}

void Dialog::fecharAlerta(){
  if(DEBUG_CONSOLE) {
    qDebug() << "close !";
  }
  this->close();
  QApplication::exit();
}

void Dialog::on_pushButton_clicked()
{
 this->fecharAlerta();
}

void Dialog::setTimeout(int timeout)
{
  m_timeout = timeout;
}

int Dialog::timeout()
{
  return m_timeout;
}

//Inicializa as variaveis.
bool Dialog::setParams(int argc, char *argv[]){
    QHash<QByteArray, QString>  params;
    int i;
    for(i=0; i < argc; i++) {
      QByteArray value = argv[i];
      if(value.startsWith('-')) {
        params[value.mid(1)] = argv[i+1];
      }
    }

   this->setTimeout(params.value("timeout", "7").toInt());
   this->setTitle(params.value("titulo",  "Atenção"));
   QString texto = params.value("texto",   "Texto de Teste");
   texto = texto.replace(QRegExp("&#039;"), "");
   texto = texto.replace(QRegExp("&lt;"), "<");
   texto = texto.replace(QRegExp("&gt;"), ">");
   texto = texto.replace(QRegExp("&quot;"), "\"");
   texto = texto.replace(QRegExp("&amp;"), "&");
   this->sendNotify(texto);
   return true;
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    }
}
