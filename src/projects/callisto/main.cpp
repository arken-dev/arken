#include "dialog.h"
#include <iostream>
#include <QApplication>
#include <QDebug>
#include <callisto.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Callisto callisto(argc, argv, app.applicationFilePath().toLocal8Bit().data());
    return app.exec();
}
