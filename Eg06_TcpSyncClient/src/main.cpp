#include <iostream>
#include "BlockingClient.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Fortune Synchronous BlockingClient");
    BlockingClient server;
    server.show();

    return app.exec();
}