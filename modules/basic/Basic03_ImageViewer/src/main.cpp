#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // QImage image("./data/img.jpg");
    QImage image("./data/img1.png");

    MainWindow mainWindow;
    mainWindow.setMinimumSize(400, 500);
    mainWindow.show();
    mainWindow.setImage(image);

    return app.exec();
}