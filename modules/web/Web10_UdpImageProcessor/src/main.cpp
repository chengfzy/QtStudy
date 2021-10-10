#include "Processor.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Image Processor");
    Processor processor;
    processor.show();

    return app.exec();
}