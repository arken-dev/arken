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
    //setAttribute(Qt::WA_ShowWithoutActivating);

    const int width = QApplication::desktop()->width();
    this->move(width-400,0);
    QFile file(QDir::tempPath() + "/notify");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.close();
    m_bootTime = QDateTime::currentDateTime();
    QString tmp_file = (QDir::tempPath() + "/notify");
    //KeyLoggerWorker * keylogger = new KeyLoggerWorker(tmp_file);
    //keylogger->start();
    this->startTimer(100);
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

// Permanesse rodando até currentTime atingir o Timeout
// Após isto fecha aplicaçao
void Dialog::timerEvent(QTimerEvent *event)
{
    if(DEBUG_CONSOLE) {
      qDebug() << (QDateTime::currentMSecsSinceEpoch() - m_currenttime) / 1000.0;
    }

    if( m_currenttime > 0 and ((QDateTime::currentMSecsSinceEpoch() - m_currenttime)/1000.0) > m_timeout ) {
      this->m_currenttime = 0;
      this->hide();
    }
}

void Dialog::fecharAlerta(){
  if(DEBUG_CONSOLE) {
    qDebug() << "close !";
  }
  this->hide();
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

void Dialog::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Escape)
  {
    this->hide();
  }
}

void Dialog::send(QString title, QString body)
{
  this->hide();
  ui->label->setText(title);
  ui->notify_label->setText(body);
  this->resizeMsg();
  m_currenttime = QDateTime::currentMSecsSinceEpoch();
  this->show();
}
