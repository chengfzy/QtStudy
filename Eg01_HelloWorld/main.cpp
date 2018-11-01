#include <iostream>
#include "QtWidgets/QtWidgets"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QMainWindow* mainWindow = new QMainWindow;
    mainWindow->setFixedSize(QSize(500, 400));
    mainWindow->show();

    return app.exec();
}