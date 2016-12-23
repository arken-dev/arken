// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDateTime>
#include <QFont>
#include <QJsonObject>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

signals:
    void clicked();

public:
    bool m_autoclose   = true;
    qint64 m_currenttime = 0;
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void mouseReleaseEvent(QMouseEvent * event);
    void fecharAlerta();
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void send(QString icon, QString title, QString body);

private slots:
    bool resizeMsg();

private:
    int  m_timeout = 10;
    Ui::Dialog *ui;
    QFont buildFont(QJsonObject object);
};

#endif // DIALOG_H
