#include "QApplication"
#include "Receiver.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Receiver receiver;
    receiver.show();

    return app.exec();
}