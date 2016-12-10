// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "dialog.h"
#include "ui_dialog.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QHash>
#include <QDir>
#include <QFile>
#include <QKeyEvent>
#include <QFile>

#define DEBUG_CONSOLE false

#include <keyloggerworker.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    #ifdef Q_OS_MAC
    setWindowFlags(Qt::SubWindow
      | Qt::FramelessWindowHint
      | Qt::WindowSystemMenuHint
      | Qt::WindowStaysOnTopHint // remove porque ele não mostra uma janela em cima da outra
    );
    #endif

    #ifdef Q_OS_LINUX
    setWindowFlags(Qt::Window
      | Qt::WindowTitleHint
      | Qt::CustomizeWindowHint
      | Qt::FramelessWindowHint
      | Qt::SplashScreen //Ele mantem em backgroud mesmo quand otermina a aplicaçao.
    );
    #endif

    #ifdef Q_OS_WIN
    setWindowFlags(Qt::Window
      | Qt::WindowTitleHint
      | Qt::CustomizeWindowHint
      | Qt::FramelessWindowHint
      | Qt::WindowStaysOnTopHint // remove porque ele não mostra uma janela em cima da outra
      | Qt::SplashScreen //Ele mantem em backgroud mesmo quand otermina a aplicaçao.
    );
    #endif


    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //setAttribute(Qt::WA_ShowWithoutActivating);

    const int width = QApplication::desktop()->width();
    this->move(width-400, 0);
    this->startTimer(100);

    QString fileName("");

    if( QFile::exists("config/callisto.json") ) {
      fileName = "config/callisto.json";
    }

    if( QFile::exists("config/callisto.json.user") ) {
      fileName = "config/callisto.json.user";
    }

    QFile config(fileName);

    if( !fileName.isEmpty() && config.exists() ) {
      config.open(QIODevice::ReadOnly);
      QJsonParseError * error = new QJsonParseError();
      QJsonDocument json = QJsonDocument::fromJson(config.readAll(), error);
      if( error->error != 0 ) {
        qDebug() << error->errorString();
        throw;
      }
      QJsonObject object = json.object();

      QJsonObject title  = object.value("title").toObject();
      QString colorTitle = title.value("color").toString();
      QFont    fontTitle = buildFont(title.value("font").toObject());
      colorTitle.prepend("background-color: ");
      colorTitle.append(";");
      ui->label->setFont(fontTitle);
      ui->topo->setStyleSheet(colorTitle);
      ui->label->setStyleSheet(ui->label->styleSheet().append(colorTitle));

      QJsonObject body  = object.value("body").toObject();
      QString colorBody = body.value("color").toString();
      QFont    fontBody = buildFont(body.value("font").toObject());
      colorBody.prepend("background-color: ");
      colorBody.append(";");
      ui->notify_label->setFont(fontBody);
      ui->conteudo->setStyleSheet(colorBody);
      ui->notify_label->setStyleSheet(ui->notify_label->styleSheet().append(colorBody));
    }

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

QFont Dialog::buildFont(QJsonObject object)
{
  QFont font(object.value("name").toString());
  font.setPointSize(object.value("size").toInt());
  font.setBold(object.value("bold").toBool());
  if( object.value("antialias").toBool() ) {
    font.setStyleStrategy(QFont::PreferAntialias);
  }
  return font;
}

