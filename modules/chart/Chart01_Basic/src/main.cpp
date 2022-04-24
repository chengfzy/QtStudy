#include <QApplication>
#include <QTableView>
#include "MainWidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWidget w;
    w.resize(720, 480);
    w.show();

    return app.exec();
}