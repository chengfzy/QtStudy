#include <QApplication>
#include "ImageGraphicsView.h"
#include "ImageWidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // QImage image("./data/img.jpg");
    QImage image("./data/img1.png");

    ImageWidget imageWidget;
    imageWidget.setMinimumSize(400, 500);
    imageWidget.show();
    imageWidget.setImage(image);

    return app.exec();
}