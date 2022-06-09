#include <QCoreApplication>
#include "server.h"
#include <iostream>
#include "signal.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Server server;
    server.Run();
    return app.exec();
}
