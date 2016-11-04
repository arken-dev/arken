#include "dialog.h"
#include <iostream>
#include <QApplication>
#include <QDebug>
#include <puck.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Puck puck(argc, argv, app.applicationFilePath().toLocal8Bit().data());
    return app.exec();
}
