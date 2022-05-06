#include <QApplication>
#include "ImageGraphicsView.h"
#include "ImageWidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QImage image("./data/img.jpg");

    // ImageGraphicsView view;
    // view.setImage(image);
    // view.show();

    ImageWidget imageWidget;
    imageWidget.setImage(image);
    imageWidget.show();

    return app.exec();
}