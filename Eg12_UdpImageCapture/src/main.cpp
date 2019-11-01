#include "Capture.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Image Capture");
    Capture capture;
    capture.show();

    return app.exec();
}