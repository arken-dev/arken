#include "dialog.h"
#include <iostream>
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setParams(argc, argv);
    w.show();
    w.focusWidget();

    //qDebug() << "teste";
    return a.exec();
}
