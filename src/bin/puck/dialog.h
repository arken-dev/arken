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
    int  m_currenttime = 0;
    QDateTime m_bootTime;
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void mouseReleaseEvent(QMouseEvent * event);
    bool sendNotify(QString texto);
    void fecharAlerta();
    void showEvent ( QShowEvent * event );
    void timerEvent(QTimerEvent *event);
    void setTimeout(int tempo);
    int  timeout();
    bool setTitle(QString texto);
    bool setParams(int argc, char *argv[]);
private slots:
    bool resizeMsg();
    void on_pushButton_clicked();

private:
    int  m_timeout = 7;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
