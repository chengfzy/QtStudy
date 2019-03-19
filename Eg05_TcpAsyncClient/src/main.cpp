#include <iostream>
#include "Client.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Fortune Client");
    Client server;
    server.show();

    return app.exec();
}