#include <Widget.h>
#include <QApplication>
#include <QSurfaceFormat>
#include "Window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

    Window window;
    window.show();

    return app.exec();
}