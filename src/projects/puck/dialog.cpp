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
    this->move(width-400, 0);
    this->startTimer(100);
}

void Dialog::mouseReleaseEvent(QMouseEvent * event){
    if(DEBUG_CONSOLE) {
      qDebug() << "mouse event";
      qDebug() << event ;
    }
    this->fecharAlerta();
 }

Dialog::~Dialog()
{
    delete ui;
}

//Redimensiona a janela
bool Dialog::resizeMsg(){
    ui->notify_label->adjustSize();
    ui->label->adjustSize();

    int sizeGroup = ui->notify_label->height();
    ui->principal->move(0,0);
    ui->conteudo->setFixedHeight(sizeGroup + 65);

    ui->principal->setFixedHeight(sizeGroup + 65);
    this->setFixedHeight(sizeGroup + 65);

    return true;
}

// Permanesse rodando até currentTime atingir o Timeout
// Após isto fecha aplicaçao
void Dialog::timerEvent(QTimerEvent *)
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

void Dialog::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Escape)
  {
    this->hide();
  }
}

void Dialog::send(QString icon, QString title, QString body)
{
  icon.prepend(":/images/");
  icon.append(".png");
  this->hide();
  int index = title.lastIndexOf("/");
  if( index == -1 ) {
    title.lastIndexOf("\\");
  }
  if( index == -1 ) {
    index = 0;
  } else {
    index++;
  }
  ui->label->setText(title.mid(index, title.size()));
  ui->notify_label->setText(body);
  this->resizeMsg();
  m_currenttime = QDateTime::currentMSecsSinceEpoch();
  QPixmap image = QPixmap(icon);
  ui->icon->setPixmap(image.scaled(ui->icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  this->show();
}
