#include <QDebug>
#include <QtCore>
#include <QCoreApplication>
#include "mirandaserver.h"

int main(int argc, char * argv[])
{
   QCoreApplication *app = new QCoreApplication(argc, argv);

   MirandaServer server(app);

   return app->exec();
}
