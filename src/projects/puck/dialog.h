#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDateTime>

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
    void on_pushButton_clicked();

private:
    int  m_timeout = 10;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
