#include "dialog.h"
#include <iostream>
#include <QApplication>
#include <QDebug>
#include <puck.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Puck puck(app);
    /*
    Dialog w;
    w.setParams(argc, argv);
    w.show();
    */
    return app.exec();
}
