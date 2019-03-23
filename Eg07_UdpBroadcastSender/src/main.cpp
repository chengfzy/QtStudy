#include <iostream>
#include "Sender.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Fortune Asynchronous Client");
    Sender sender;
    sender.show();

    return app.exec();
}