#include <iostream>
#include "Receiver.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Fortune Asynchronous Client");
    Receiver receiver;
    receiver.show();

    return app.exec();
}