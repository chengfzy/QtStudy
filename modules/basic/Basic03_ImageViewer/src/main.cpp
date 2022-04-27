#include <QApplication>
#include "ImageGraphicsView.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QImage image("./data/img.jpg");

    ImageGraphicsView view;
    view.setImage(image);
    view.show();

    return app.exec();
}