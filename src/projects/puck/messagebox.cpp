#include "messagebox.h"
#include <QMessageBox>
class MessageBox : public QMessageBox
        int timeout;
        bool autoClose;
        int currentTime;

        void MessageBox::showEvent ( QShowEvent * event ) {
            currentTime = 0;
            if (autoClose) {
            this->startTimer(1000);
            }
        }

        void MessageBox::timerEvent(QTimerEvent *event)
        {
            currentTime++;
            if (currentTime>=timeout) {
            this->done(0);
            }
        }

