#include <iostream>
#include "Server.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Fortune Server");
    Server server;
    server.show();

    return app.exec();
}